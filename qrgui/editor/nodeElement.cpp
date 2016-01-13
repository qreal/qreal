/* Copyright 2007-2016 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "nodeElement.h"

#include <QtCore/QUuid>
#include <QtCore/QtMath>
#include <QtWidgets/QStyle>
#include <QtWidgets/QStyleOptionGraphicsItem>
#include <QtWidgets/QMessageBox>
#include <QtGui/QTextCursor>
#include <QtWidgets/QToolTip>
#include <QtWidgets/QGraphicsDropShadowEffect>

#include <math.h>
#include <qrkernel/logging.h>

#include <qrgui/models/models.h>
#include <qrgui/models/commands/changeParentCommand.h>
#include <qrgui/models/commands/renameCommand.h>
#include <qrgui/plugins/editorPluginInterface/editorInterface.h>

#include "editor/labels/label.h"
#include "editor/labels/labelFactory.h"
#include "editor/editorViewScene.h"
#include "editor/ports/portFactory.h"

#include "editor/private/resizeHandler.h"

#include "editor/commands/resizeCommand.h"
#include "editor/commands/foldCommand.h"
#include "editor/commands/insertIntoEdgeCommand.h"

using namespace qReal;
using namespace qReal::commands;
using namespace qReal::gui::editor;
using namespace qReal::gui::editor::commands;

NodeElement::NodeElement(ElementImpl *impl, const Id &id, const models::Models &models)
	: Element(impl, id, models)
	, mExploser(models.exploser())
	, mSwitchGridAction(tr("Switch on grid"), this)
	, mDragState(None)
	, mResizeCommand(nullptr)
	, mIsExpanded(false)
	, mIsFolded(false)
	, mLeftPressed(false)
	, mPos(QPointF(0,0))
	, mSelectionNeeded(false)
	, mConnectionInProgress(false)
	, mPlaceholder(nullptr)
	, mHighlightedNode(nullptr)
	, mRenderTimer(this)
{
	setAcceptHoverEvents(true);
	setFlag(ItemClipsChildrenToShape, false);
	setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren);

	LabelFactory labelFactory(models.graphicalModelAssistApi(), mId);
	QList<LabelInterface*> titles;

	QList<PortInterface *> ports;
	PortFactory portFactory;
	mElementImpl->init(mContents, portFactory, ports, labelFactory, titles, &mRenderer, this);
	mPortHandler = new PortHandler(this, mGraphicalAssistApi, ports);

	for (LabelInterface * const labelInterface : titles) {
		Label * const label = dynamic_cast<Label *>(labelInterface);
		if (!label) {
			continue;
		}

		label->init(mContents);
		label->setParentItem(this);
		mLabels.append(label);
	}

	mFoldedContents = mContents;

	mSwitchGridAction.setCheckable(true);
	connect(&mSwitchGridAction, SIGNAL(toggled(bool)), this, SLOT(switchGrid(bool)));

	mGrid = new SceneGridHandler(this);
	switchGrid(SettingsManager::value("ActivateGrid").toBool());

	initPortsVisibility();

	connect(&mRenderTimer, SIGNAL(timeout()), this, SLOT(initRenderedDiagram()));
}

NodeElement::~NodeElement()
{
	for (EdgeElement * const edge : mEdgeList) {
		edge->removeLink(this);
	}

	deleteGuides();
	qDeleteAll(mLabels);
	delete mElementImpl;
	delete mGrid;
	delete mPortHandler;
}

void NodeElement::initPortsVisibility()
{
	for (const QString &portType : mGraphicalAssistApi.editorManagerInterface().portTypes(id().type())) {
		mPortsVisibility.insert(portType, false);
	}
}

void NodeElement::connectSceneEvents()
{
	if (!scene()) {
		return;
	}

	const EditorView *editorView = nullptr;
	for (const QGraphicsView *view : scene()->views()) {
		if ((editorView = dynamic_cast<const EditorView *>(view))) {
			break;
		}
	}

	updateBySelection();
	mRenderer.setZoom(editorView->transform().m11());
	if (editorView) {
		connect(editorView, &EditorView::zoomChanged, &mRenderer, &SdfRenderer::setZoom);
	}
}

QMap<QString, QVariant> NodeElement::graphicalProperties() const
{
	return mGraphicalAssistApi.properties(id());
}

QMap<QString, QVariant> NodeElement::logicalProperties() const
{
	return mGraphicalAssistApi.properties(logicalId());
}

void NodeElement::setName(const QString &value, bool withUndoRedo)
{
	AbstractCommand *command = new RenameCommand(mGraphicalAssistApi, id(), value, &mExploser);
	if (withUndoRedo) {
		mController->execute(command);
		// Controller will take ownership
	} else {
		command->redo();
		delete command;
	}
}

void NodeElement::setGeometry(const QRectF &geom)
{
	prepareGeometryChange();
	setPos(geom.topLeft());
	if (geom.isValid()) {
		mContents = geom.translated(-geom.topLeft());
	}
	mTransform.reset();
	mTransform.scale(mContents.width(), mContents.height());
	adjustLinks();
}

void NodeElement::setPos(const QPointF &pos)
{
	if (qIsNaN(pos.x()) || qIsNaN(pos.y())) {
		setPos(QPointF());
		mContents.moveTo(QPointF());
		storeGeometry();
		QLOG_WARN() << "NaN passed to NodeElement::setPos(). That means that something went wrong. "\
				"Learn to reproduce this message. The position has been set to (0,0). Attend element with id" << id();
	} else {
		mPos = pos;
		QGraphicsItem::setPos(pos);
	}
}

void NodeElement::setPos(qreal x, qreal y)
{
	setPos(QPointF(x, y));
}

void NodeElement::adjustLinks()
{
	for (EdgeElement *edge : mEdgeList) {
		edge->adjustLink();
	}

	for (QGraphicsItem *child : childItems()) {
		NodeElement *element = dynamic_cast<NodeElement*>(child);
		if (element) {
			element->adjustLinks();
		}
	}
}

void NodeElement::arrangeLinearPorts()
{
	mPortHandler->arrangeLinearPorts();
	adjustLinks();
}

void NodeElement::arrangeLinks()
{
	//Episode I: Home Jumps
	for (EdgeElement* edge : mEdgeList) {
		NodeElement* src = edge->src();
		NodeElement* dst = edge->dst();
		edge->reconnectToNearestPorts(this == src, this == dst);
	}

	//Episode II: Home Ports Arranging
	arrangeLinearPorts();

	//Episode III: Remote Jumps
	for (EdgeElement* edge : mEdgeList) {
		NodeElement* src = edge->src();
		NodeElement* dst = edge->dst();
		NodeElement* other = edge->otherSide(this);
		edge->reconnectToNearestPorts(other == src, other == dst);
	}

	//Episode IV: Remote Arrangigng
	QSet<NodeElement*> arranged;
	for (EdgeElement* edge : mEdgeList) {
		NodeElement* other = edge->otherSide(this);
		if (other && !arranged.contains(other)) {
			other->arrangeLinearPorts();
			arranged.insert(other);
		}
	}
}

void NodeElement::storeGeometry()
{
	const QPolygon contents(mContents.toAlignedRect()); // saving correct current contents

	if ((pos() != mGraphicalAssistApi.position(id()))) { // check if it's been changed
		mGraphicalAssistApi.setPosition(id(), pos());
	}

	if (contents != mGraphicalAssistApi.configuration(id())) { // check if it's been changed
		mGraphicalAssistApi.setConfiguration(id(), contents);
	}
}

void NodeElement::showAlignment(bool isChecked)
{
	mGrid->setShowAlignmentMode(isChecked);
}

void NodeElement::switchAlignment(bool isSwitchedOn)
{
	mGrid->setAlignmentMode(isSwitchedOn);
}

void NodeElement::switchGrid(bool isChecked)
{
	mGrid->setGridMode(isChecked);
	mSwitchGridAction.setChecked(isChecked);
	if (isChecked) {
		alignToGrid();

		for (EdgeElement * const edge : mEdgeList) {
			edge->alignToGrid();
		}
	}
}

void NodeElement::deleteGuides()
{
	mGrid->deleteGuides();
}

void NodeElement::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::RightButton) {
		event->accept();
		return;
	}

	startResize();
	if (isSelected()) {
		int dragArea = SettingsManager::instance()->value("DragArea").toInt();
		if (QRectF(mContents.topLeft(), QSizeF(dragArea, dragArea)).contains(event->pos())
				&& mElementImpl->isResizeable())
		{
			mDragState = TopLeft;
		} else if (QRectF(mContents.topRight(), QSizeF(-dragArea, dragArea)).contains(event->pos())
				&& mElementImpl->isResizeable())
		{
			mDragState = TopRight;
		} else if (QRectF(mContents.bottomRight(), QSizeF(-dragArea, -dragArea)).contains(event->pos())
				&& mElementImpl->isResizeable())
		{
			mDragState = BottomRight;
		} else if (QRectF(mContents.bottomLeft(), QSizeF(dragArea, -dragArea)).contains(event->pos())
				&& mElementImpl->isResizeable())
		{
			mDragState = BottomLeft;
		} else if (QRectF(QPointF(-20, 0), QPointF(0, 20)).contains(event->pos())
				&& mElementImpl->isContainer())
		{
			changeFoldState();
		} else {
			Element::mousePressEvent(event);
		}
	} else {
		Element::mousePressEvent(event);
	}

	mDragPosition = event->scenePos() - scenePos();

	mLeftPressed = true;
	setZValue(1);

	if (flags() & ItemIsMovable) {
		recalculateHighlightedNode(event->scenePos());
	}
}

void NodeElement::alignToGrid()
{
	if (SettingsManager::value("ActivateGrid").toBool()) {
		NodeElement *parent = dynamic_cast<NodeElement *>(parentItem());
		if (!parent || !parent->mElementImpl->isSortingContainer()) {
			mGrid->alignToGrid();
		}
	}
}

void NodeElement::recalculateHighlightedNode(const QPointF &mouseScenePos)
{
	// in case of unresizable item use switch
	// Determing parent using corner position, not mouse coordinates
	QPointF newParentInnerPoint = mouseScenePos;
	switch (mDragState) {
		case TopLeft:
			newParentInnerPoint = scenePos();
			break;
		case Top:
			newParentInnerPoint = scenePos() + QPointF(mContents.width() / 2, 0);
			break;
		case TopRight:
			newParentInnerPoint = scenePos() + QPointF(mContents.width(), 0);
			break;
		case Left:
			newParentInnerPoint = scenePos() + QPointF(0, mContents.height() / 2);
			break;
		case Right:
			newParentInnerPoint = scenePos() + QPointF(mContents.width(), mContents.height() / 2);
			break;
		case BottomLeft:
			newParentInnerPoint = scenePos() + QPointF(0, mContents.height());
			break;
		case Bottom:
			newParentInnerPoint = scenePos() + QPointF(mContents.width() / 2, mContents.height());
			break;
		case BottomRight:
			newParentInnerPoint = scenePos() + QPointF(mContents.width(), mContents.height());
			break;
		case None:
			break;
	}

	EditorViewScene *evScene = dynamic_cast<EditorViewScene*>(scene());
	NodeElement *newParent = evScene->findNewParent(newParentInnerPoint, this);

	// it would be nice optimization to do nothing in case of
	// mHighlightedNode == newParent, but it's unapplicable here because
	// of element could be moved inside his parent

	if (newParent) {
		if (mHighlightedNode) {
			mHighlightedNode->erasePlaceholder(false);
		}
		mHighlightedNode = newParent;
		mHighlightedNode->drawPlaceholder(EditorViewScene::getPlaceholder(), mouseScenePos);
	} else if (mHighlightedNode != nullptr) {
		mHighlightedNode->erasePlaceholder(true);
		mHighlightedNode = nullptr;
	}
}

void NodeElement::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::RightButton) {
		event->accept();
		return;
	}

	// Folded elements can't be resized.
	// So drag state isn't important.
	if (mIsFolded) {
		mDragState = None;
	}

	QRectF newContents = mContents;
	QPointF newPos = mPos;

	// expanding containers are turned off
	// TODO: bring them back (but not their bugs =))
	bool needResizeParent = false;

	if (mDragState == None) {
		if (!(flags() & ItemIsMovable)) {
			return;
		}

		recalculateHighlightedNode(event->scenePos());

		// it is needed for sendEvent() to every isSelected element thro scene
		event->setPos(event->lastPos());

		Element::mouseMoveEvent(event);

		mGrid->mouseMoveEvent(event);
		alignToGrid();
		newPos = pos();
	} else if (mElementImpl->isResizeable()) {
		setVisibleEmbeddedLinkers(false);

		needResizeParent = true;

		QPointF parentPos = QPointF(0, 0);
		QGraphicsItem *parItem = parentItem();
		if (parItem) {
			parentPos = parItem->scenePos();
		}

		qreal newX = event->pos().x();
		qreal newY = event->pos().y();

		switch (mDragState) {
		case TopLeft: {
			newContents.setTopLeft(event->pos() - event->lastPos());
			newPos = event->scenePos() - parentPos;
			break;
		}
		case Top: {
			newContents.setTop(newY);
			newPos = QPoint(pos().x(), event->scenePos().y() - parentPos.y());
			break;
		}
		case TopRight: {
			newContents.setTopRight(QPoint(newX, event->pos().y() - event->lastPos().y()));
			newPos = QPoint(newPos.x(), event->scenePos().y() - parentPos.y());
			break;
		}
		case Left: {
			newContents.setLeft(newX);
			newPos = QPoint(event->scenePos().x() - parentPos.x(), pos().y());
			break;
		}
		case Right: {
			newContents.setRight(newX);
			break;
		}
		case BottomLeft: {
			newContents.setBottomLeft(QPoint(event->pos().x() - event->lastPos().x(), newY));
			newPos = QPoint(event->scenePos().x() - parentPos.x(), pos().y());
			break;
		}
		case Bottom: {
			newContents.setBottom(newY);
			break;
		}
		case BottomRight: {
			newContents.setBottomRight(QPoint(newX, newY));
			break;
		}
		case None:
			break;
		}

		if (event->modifiers() & Qt::ShiftModifier) {
			const qreal size = qMax(newContents.width(), newContents.height());
			newContents.setWidth(size);
			newContents.setHeight(size);
		}
	}

	resize(newContents, newPos, needResizeParent);
}

void NodeElement::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if (dynamic_cast<NodeElement *>(scene()->mouseGrabberItem()) == this) {
		ungrabMouse();
	}

	if (event->button() == Qt::RightButton) {
		event->accept();
		return;
	}
	deleteGuides();

	storeGeometry();

	if (scene() && (scene()->selectedItems().size() == 1) && isSelected()) {
		setVisibleEmbeddedLinkers(true);
	}

	if (mDragState == None) {
		Element::mouseReleaseEvent(event);
	}

	EditorViewScene *evScene = dynamic_cast<EditorViewScene *>(scene());
	commands::InsertIntoEdgeCommand *insertCommand = new commands::InsertIntoEdgeCommand(
			*evScene, mModels, id(), id(), Id::rootId(), event->scenePos(), boundingRect().bottomRight(), false);

	bool shouldProcessResize = true;

	// we should use mHighlightedNode to determine if there is a highlighted node
	// insert current element into them and set mHighlightedNode to nullptr
	// but because of mouseRelease twice triggering we can't do it
	// This may cause more bugs
	if (flags() & ItemIsMovable) {
		if (mHighlightedNode) {
			NodeElement *newParent = mHighlightedNode;
			Element *insertBefore = mHighlightedNode->getPlaceholderNextElement();
			mHighlightedNode->erasePlaceholder(false);
			// commented because of bug with double event sending (see #204)
			// mHighlightedNode = nullptr;

			QPointF newPos = mapToItem(newParent, mapFromScene(scenePos()));
			AbstractCommand *parentCommand = changeParentCommand(newParent->id(), newPos);
			mController->execute(parentCommand);
			// Position change already processed in change parent command
			shouldProcessResize = parentCommand == nullptr;
			setPos(newPos);

			if (insertBefore) {
				mGraphicalAssistApi.stackBefore(id(), insertBefore->id());
			}

			newParent->resize();

			while (newParent) {
				newParent->mContents = newParent->mContents.normalized();
				newParent->storeGeometry();
				newParent = dynamic_cast<NodeElement*>(newParent->parentItem());
			}
		} else {
			AbstractCommand *parentCommand = changeParentCommand(evScene->rootItemId(), scenePos());
			mController->execute(parentCommand);
			// Position change already processed in change parent command
			shouldProcessResize = parentCommand == nullptr;
		}
	}

	for (EdgeElement* edge : mEdgeList) {
		edge->layOut();
		if (SettingsManager::value("ActivateGrid").toBool()) {
			edge->alignToGrid();
		}
	}

	if (shouldProcessResize && mResizeCommand) {
		mResizeCommand->addPostAction(insertCommand);
		endResize();
	}

	updateBySelection();
	mDragState = None;
}

void NodeElement::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	Q_UNUSED(event);

	if (mElementImpl->isContainer()) {
		mController->execute(new FoldCommand(this));
	}
}

void NodeElement::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	Q_UNUSED(event);
	update();
}

void NodeElement::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	Q_UNUSED(event);
}

void NodeElement::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	Q_UNUSED(event);
	update();
}

void NodeElement::startResize()
{
	mResizeCommand = new ResizeCommand(dynamic_cast<EditorViewScene *>(scene()), id());
	mResizeCommand->startTracking();
}

void NodeElement::endResize()
{
	if (mResizeCommand) {
		mResizeCommand->stopTracking();
		if (mResizeCommand->modificationsHappened()) {
			mController->execute(mResizeCommand);
		} else {
			delete mResizeCommand;
		}

		// Undo stack took ownership
		mResizeCommand = nullptr;
	}
}

bool NodeElement::connectionInProgress()
{
	return mConnectionInProgress;
}

void NodeElement::setConnectingState(bool arg)
{
	mConnectionInProgress = arg;
}

bool NodeElement::initPossibleEdges()
{
	if (!mPossibleEdges.isEmpty()) {
		return true;
	}

	const QStringList portTypes = mGraphicalAssistApi.editorManagerInterface().portTypes(id().type());
	for (const QString &elementName : mGraphicalAssistApi.editorManagerInterface().elements(id().editor()
			, id().diagram())) {
		int ne = mGraphicalAssistApi.editorManagerInterface().isNodeOrEdge(id().editor(), elementName);
		if (ne == -1) {
			const QList<StringPossibleEdge> list = mGraphicalAssistApi.editorManagerInterface()
					.possibleEdges(id().editor(), elementName);
			for (const StringPossibleEdge &pEdge : list) {
				if (portTypes.contains(pEdge.first.first)
						|| (portTypes.contains(pEdge.first.second) && !pEdge.second.first))
				{
					PossibleEdgeType edge(pEdge.second.first, Id(id().editor(), id().diagram(), pEdge.second.second));
					const QSet<ElementPair> elementPairs = elementsForPossibleEdge(pEdge);
					if (elementPairs.empty()) {
						continue;
					}

					for (const ElementPair &elementPair : elementPairs) {
						mPossibleEdges.insert(qMakePair(elementPair, edge));
					}

					mPossibleEdgeTypes.insert(edge);
				}
			}
		}
	}

	return !mPossibleEdges.isEmpty();
}

void NodeElement::initEmbeddedLinkers()
{
	if (!mEmbeddedLinkers.isEmpty()) {
		return;
	}
	QSet<Id> usedEdges;
	for (const PossibleEdgeType &type : mPossibleEdgeTypes) {
		if (usedEdges.contains(type.second)) {
			continue;
		}

		EmbeddedLinker* embeddedLinker = new EmbeddedLinker();
		scene()->addItem(embeddedLinker);
		embeddedLinker->setEdgeType(type.second);
		embeddedLinker->setDirected(type.first);
		mEmbeddedLinkers.append(embeddedLinker);
		embeddedLinker->setMaster(this);
		usedEdges.insert(type.second);
	}
	setVisibleEmbeddedLinkers(true);

	// TODO: make it customizable
	// for (EmbeddedLinker* embeddedLinker : mEmbeddedLinkers) {
	// embeddedLinker->initTitle();
	// }
}

void NodeElement::setVisibleEmbeddedLinkers(const bool show)
{
	if (show) {
		setZValue(250);
		int index = 0;
		int maxIndex = mEmbeddedLinkers.size();
		for (EmbeddedLinker* embeddedLinker : mEmbeddedLinkers) {
			embeddedLinker->takePosition(index,maxIndex);
			embeddedLinker->show();
			index++;
		}
	} else {
		setZValue(0);
		for (EmbeddedLinker* embeddedLinker : mEmbeddedLinkers) {
			embeddedLinker->hide();
		}
	}
}

QVariant NodeElement::itemChange(GraphicsItemChange change, const QVariant &value)
{
	bool isItemAddedOrDeleted = false;
	NodeElement *item = dynamic_cast<NodeElement*>(value.value<QGraphicsItem*>());
	switch (change) {
	case ItemPositionHasChanged:
		if (mDragState == None) {
			alignToGrid();
		}

		if (isSelected()) {
			/// @todo: Actually we must do it when user drags items group. For now the criteria of that is selection
			/// state, but we need to change it to more trustful one.
			storeGeometry();
		}

		adjustLinks();
		return value;

	case ItemChildAddedChange:
	case ItemChildRemovedChange:
		isItemAddedOrDeleted = true;
		if (item)
			updateByChild(item, isItemAddedOrDeleted);
		return value;

	case ItemParentHasChanged:
		updateByNewParent();
		return value;

	case ItemSelectedChange: {
		if (connectionInProgress()) {
			// If we are dragging edge from linker then unselecting this element will cause dragging interruption.
			// So unselection events must be declined, doing it here...
			return QGraphicsItem::itemChange(change, true);
		}

		return QGraphicsItem::itemChange(change, value);
	}
	case ItemSelectedHasChanged: {
		updateBySelection();
		return QGraphicsItem::itemChange(change, value);
	}

	case ItemSceneHasChanged: {
		connectSceneEvents();
		return QGraphicsItem::itemChange(change, value);
	}

	default:
		return QGraphicsItem::itemChange(change, value);
	}
}

QRectF NodeElement::contentsRect() const
{
	return mContents;
}

QRectF NodeElement::boundingRect() const
{
//	qDebug() << mContents;
	return mContents.adjusted(-2 * kvadratik, -2 * kvadratik, kvadratik, kvadratik);
}

void NodeElement::updateData()
{
	Element::updateData();
	if (!mMoving) {
		QPointF newpos = mGraphicalAssistApi.position(id());
		QPolygon newpoly = mGraphicalAssistApi.configuration(id());

		QRectF newRect; // Use default ((0,0)-(0,0))
		// QPolygon::boundingRect is buggy :-(
		if (!newpoly.isEmpty()) {
			int minx = newpoly[0].x();
			int miny = newpoly[0].y();
			int maxx = newpoly[0].x();
			int maxy = newpoly[0].y();
			for (int i = 1; i < newpoly.size(); ++i) {
				if (minx > newpoly[i].x()) {
					minx = newpoly[i].x();
				}
				if (maxx < newpoly[i].x()) {
					maxx = newpoly[i].x();
				}
				if (miny > newpoly[i].y()) {
					miny = newpoly[i].y();
				}
				if (maxy < newpoly[i].y()) {
					maxy = newpoly[i].y();
				}
			}
			newRect = QRectF(QPoint(minx, miny), QSize(maxx - minx, maxy - miny));
		}

		setGeometry(newRect.translated(newpos));
	}
	mElementImpl->updateData(this);
	updateLabels();
	update();
}

const QPointF NodeElement::portPos(qreal id) const
{
	return mPortHandler->portPos(id);
}

bool NodeElement::isContainer() const
{
	return mElementImpl->isContainer();
}

int NodeElement::numberOfPorts() const
{
	return mPortHandler->numberOfPorts();
}

qreal NodeElement::portId(const QPointF &location, const QStringList &types) const
{
	return mPortHandler->portId(location, types);
}

QPointF NodeElement::closestPortPoint(const QPointF &location, const QStringList &types) const
{
	return mapToScene(mPortHandler->nearestPort(location, types));
}

void NodeElement::setPortsVisible(const QStringList &types)
{
	prepareGeometryChange();

	for (const QString &portType : mPortsVisibility.keys()) {
		mPortsVisibility[portType] = types.contains(portType);
	}
}

void NodeElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *style, QWidget *)
{
	mElementImpl->paint(painter, mContents);
	paint(painter, style);

	if (mSelectionNeeded) {
		painter->save();
		painter->setPen(QPen(Qt::blue));
		QRectF rect = boundingRect();
		qreal x1 = rect.x() + 9;
		qreal y1 = rect.y() + 9;
		qreal x2 = rect.x() + rect.width() - 9;
		qreal y2 = rect.y() + rect.height() - 9;
		painter->drawRect(QRectF(QPointF(x1, y1), QPointF(x2, y2)));
		painter->restore();
	}
}

void NodeElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
	if (option->levelOfDetail >= 0.5) {
		if (option->state & QStyle::State_Selected) {
			painter->save();

			QBrush b;

			if (mElementImpl->isContainer()) {
				b.setStyle(Qt::NoBrush);
				painter->setBrush(b);

				painter->drawRect(QRectF(QPointF(-20, 0), QPointF(0, 20)));

				painter->drawLine(-15, 10, -5, 10);
				if (mIsFolded) {
					painter->drawLine(-10, 5, -10, 15);
				}
			}

			b.setStyle(Qt::SolidPattern);
			b.setColor(Qt::blue);
			painter->setBrush(b);
			painter->setPen(Qt::blue);

			if (mElementImpl->isResizeable()) {
				drawLinesForResize(painter);
			} else {
				painter->drawRect(QRectF(mContents.bottomRight(), QSizeF(-4, -4)));
			}

			painter->restore();
		}

		drawPorts(painter, option->state & QStyle::State_MouseOver);

		if (mIsExpanded && mLogicalAssistApi.logicalRepoApi().outgoingExplosion(logicalId()) != Id()) {
			QRectF rect = diagramRenderingRect();
			painter->drawImage(rect, mRenderedDiagram.scaled(rect.size().toSize()
					, Qt::KeepAspectRatio, Qt::SmoothTransformation));
		}
	}
}

void NodeElement::drawPorts(QPainter *painter, bool mouseOver)
{
	painter->save();
	painter->setOpacity(0.7);

	const QStringList portTypes = mouseOver ? mGraphicalAssistApi.editorManagerInterface().portTypes(id().type())
			: mPortsVisibility.keys(true);
	mPortHandler->drawPorts(painter, mContents, portTypes);

	painter->restore();
}

QList<EdgeElement*> NodeElement::getEdges() const
{
	return mEdgeList;
}

void NodeElement::addEdge(EdgeElement *edge)
{
	if (!mEdgeList.contains(edge)) {
		mEdgeList << edge;
	}
	arrangeLinearPorts();
}

void NodeElement::delEdge(EdgeElement *edge)
{
	mEdgeList.removeAll(edge);
	arrangeLinearPorts();
}

void NodeElement::changeExpanded()
{
	mIsExpanded = !mIsExpanded;
	if (mIsExpanded) {
		mRenderTimer.start(1000);
		initRenderedDiagram();
	} else {
		mRenderTimer.stop();
	}
	mGraphicalAssistApi.mutableGraphicalRepoApi().setProperty(mId, "expanded", mIsExpanded ? "true" : "false");
}

void NodeElement::changeFoldState()
{
	mIsFolded = !mIsFolded;

	for (QGraphicsItem* childItem : childItems()) {
		NodeElement* curItem = dynamic_cast<NodeElement*>(childItem);
		if (curItem) {
			curItem->setVisible(!mIsFolded);
			curItem->setLinksVisible(!mIsFolded);
		}
	}

	if (mIsFolded) {
		mCurUnfoldedContents = mContents;
		mFoldedContents.moveTo(pos());
		setGeometry(mFoldedContents);
	}
	else {
		mCurUnfoldedContents.moveTo(pos());
		setGeometry(mCurUnfoldedContents);
	}
	mGraphicalAssistApi.mutableGraphicalRepoApi().setProperty(mId, "folded", mIsFolded ? "true" : "false");

	NodeElement* parent = dynamic_cast<NodeElement*>(parentItem());
	if (parent) {
		parent->resize();
	}
	updateLabels();
}

void NodeElement::updateLabels()
{
	for (Label *title : mLabels) {
		title->setParentContents(mContents);
	}
}

void NodeElement::setLinksVisible(bool isVisible)
{
	for (EdgeElement *curEdge : mEdgeList) {
		curEdge->setVisible(isVisible);
	}

	for (QGraphicsItem* childItem : childItems()) {
		NodeElement* curItem = dynamic_cast<NodeElement*>(childItem);
		if (curItem) {
			curItem->setLinksVisible(isVisible);
		}
	}
}

void NodeElement::drawPlaceholder(QGraphicsRectItem *placeholder, QPointF pos)
{
	// for non-sorting containers no need for drawing placeholder so just make them marked
	if (!mElementImpl->isSortingContainer()) {
		setOpacity(0.2);
		return;
	}

	// binary search? No because we need to know summary height of prev elements
	NodeElement *nextItem = nullptr;

	for (QGraphicsItem* childItem : childItems()) {
		NodeElement *curItem = dynamic_cast<NodeElement*>(childItem);
		if (curItem) {
			if (curItem->scenePos().y() > pos.y()) {
				nextItem = curItem;
				break;
			}
		}
	}

	erasePlaceholder(false);
	mPlaceholder = placeholder;
	mPlaceholder->setParentItem(this);
	if(nextItem) {
		mPlaceholder->stackBefore(nextItem);
	}

	resize();
}

Element* NodeElement::getPlaceholderNextElement() const
{
	if(mPlaceholder == nullptr) {
		return nullptr;
	}
	bool found = false;
	// loking for child following the placeholder
	for (QGraphicsItem *childItem : childItems()) {
		Element *element = dynamic_cast<Element*>(childItem);
		if (found && element != nullptr) {
			return element;
		}

		if (childItem == mPlaceholder) {
			found = true;
		}
	}

	return nullptr;
}

void NodeElement::erasePlaceholder(bool redraw)
{
	setOpacity(1);

	if (!mPlaceholder) {
		return;
	}

	delete mPlaceholder;
	mPlaceholder = nullptr;

	if(redraw) {
		resize();
	}
}

void NodeElement::updateByChild(NodeElement* item, bool isItemAddedOrDeleted)
{
	if (mIsFolded && isItemAddedOrDeleted && item) {
		changeFoldState();
	}

	if (mElementImpl->isSortingContainer()) {
		updateChildrenOrder();
	}

	resize();
}

void NodeElement::updateByNewParent()
{
	NodeElement* parent = dynamic_cast<NodeElement*>(parentItem());
	if (!parent || parent->mElementImpl->hasMovableChildren()) {
		setFlag(ItemIsMovable, true);
	} else {
		setFlag(ItemIsMovable, false);
	}
}

void NodeElement::updateBySelection()
{
	initEmbeddedLinkers();
	bool singleSelected = isSelected();
	for (const QGraphicsItem *item : scene()->selectedItems()) {
		if (dynamic_cast<const Element *>(item) && item != this) {
			singleSelected = false;
			break;
		}
	}

	setVisibleEmbeddedLinkers(singleSelected);
	setHideNonHardLabels(!singleSelected && SettingsManager::value("hideNonHardLabels").toBool());
}

void NodeElement::updateChildrenOrder()
{
	QStringList ids;
	if (mGraphicalAssistApi.properties(mId).contains("childrenOrder")) {
		ids = mGraphicalAssistApi.graphicalRepoApi().property(mId, "childrenOrder").toStringList();
	}

	EditorViewScene *evScene = dynamic_cast<EditorViewScene *>(scene());
	if (evScene) {
		QStringList idsForRemoving;
		for (const QString &id : ids) {
			if (!evScene->getNodeById(Id::loadFromString(id))) {
				idsForRemoving.append(id);
			}
		}

		for (const QString &id : idsForRemoving) {
			ids.removeAll(id);
		}
	}

	QList<NodeElement *> children = childNodes();
	for (NodeElement *child : children) {
		if (!ids.contains(child->id().toString())) {
			ids << child->id().toString();
		}
	}

	mGraphicalAssistApi.mutableGraphicalRepoApi().setProperty(mId, "childrenOrder", ids);

}

QList<qreal> NodeElement::borderValues() const
{
	return mElementImpl->border();
}

QSet<ElementPair> NodeElement::elementsForPossibleEdge(const StringPossibleEdge &edge)
{
	QStringList elements = mGraphicalAssistApi.editorManagerInterface().elements(id().editor(), id().diagram());
	QStringList portTypes = mGraphicalAssistApi.editorManagerInterface().portTypes(id().type());

	QSet<ElementPair> result;
	for (const QString &element : elements) {
		QStringList otherPortTypes
				= mGraphicalAssistApi.editorManagerInterface().portTypes(Id(id().editor(), id().diagram(), element));
		if (portTypes.contains(edge.first.first) && otherPortTypes.contains(edge.first.second)) {
			result.insert(qMakePair(id().type(), Id(id().editor(), id().diagram(), element)));
		}

		if (otherPortTypes.contains(edge.first.first) && portTypes.contains(edge.first.second)) {
			result.insert(qMakePair(Id(id().editor(), id().diagram(), element), id().type()));
		}
	}

	return result;
}

QList<PossibleEdge> NodeElement::getPossibleEdges()
{
	return QList<PossibleEdge>::fromSet(mPossibleEdges);
}

void NodeElement::setColorRect(bool value)
{
	mSelectionNeeded = value;
}

void NodeElement::checkConnectionsToPort() // it is strange method
{
	mPortHandler->checkConnectionsToPort();
}

NodeInfo NodeElement::data() const
{
	NodeInfo result(id(), logicalId(), mLogicalAssistApi.parent(logicalId()), mGraphicalAssistApi.parent(id())
			, {}, {}, mLogicalAssistApi.logicalRepoApi().outgoingExplosion(logicalId())
	);

	result.setAllLogicalProperties(logicalProperties());
	result.setAllGraphicalProperties(graphicalProperties());

	// new element should not have references to links connected to original source element
	result.setGraphicalProperty("links", IdListHelper::toVariant(IdList()));
	result.setGraphicalProperty("position", mPos);

	return result;
}

void NodeElement::resize()
{
	resize(mContents, pos());
}

void NodeElement::resize(const QRectF &newContents)
{
	resize(newContents, pos());
}

void NodeElement::resize(const QRectF &newContents, const QPointF &newPos, bool needResizeParent)
{
	ResizeHandler handler(this);
	handler.resize(newContents, newPos, needResizeParent);
}

void NodeElement::drawLinesForResize(QPainter *painter)
{
	painter->save();
	painter->translate(mContents.topRight());
	drawSeveralLines(painter, -1, 1);
	painter->save();
	painter->translate(mContents.bottomRight());
	drawSeveralLines(painter, -1, -1);
	painter->save();
	painter->translate(mContents.bottomLeft());
	drawSeveralLines(painter, 1, -1);
	painter->save();
	painter->translate(mContents.topLeft());
	drawSeveralLines(painter, 1, 1);
}

void NodeElement::drawSeveralLines(QPainter *painter, int dx, int dy)
{
	int dragArea = SettingsManager::instance()->value("DragArea").toInt();
	for (int i = 1; i * 4 < dragArea + 1; i++) {
		painter->drawLine(QLineF(4 * dx * i, 0, 0, 4 * dy * i));
	}
	painter->restore();
}

bool NodeElement::isExpanded() const
{
	return mIsExpanded;
}

bool NodeElement::isFolded() const
{
	return mIsFolded;
}

QGraphicsRectItem* NodeElement::placeholder() const
{
	return mPlaceholder;
}

QRectF NodeElement::foldedContentsRect() const
{
	return mFoldedContents;
}

QList<EdgeElement *> const NodeElement::edgeList() const
{
	return mEdgeList;
}

QList<NodeElement *> const NodeElement::childNodes() const
{
	QList<NodeElement *> result;
	for (QGraphicsItem *item : childItems()) {
		NodeElement *child = dynamic_cast<NodeElement *>(item);
		if (child) {
			result << child;
		}
	}

	return result;
}

void NodeElement::updateNodeEdges()
{
	arrangeLinks();
	for (EdgeElement* edge : mEdgeList) {
		edge->adjustLink();
	}
}

AbstractCommand *NodeElement::changeParentCommand(const Id &newParent, const QPointF &position) const
{
	EditorViewScene *evScene = dynamic_cast<EditorViewScene *>(scene());
	Element *oldParentElem = dynamic_cast<Element *>(parentItem());
	const Id oldParent = oldParentElem ? oldParentElem->id() : evScene->rootItemId();
	if (oldParent == newParent) {
		return nullptr;
	}

	const QPointF oldPos = mResizeCommand ? mResizeCommand->geometryBeforeDrag().topLeft() : mPos;
	const QPointF oldScenePos = oldParentElem ? oldParentElem->mapToScene(oldPos) : oldPos;
	// Without pre-translating into new position parent gets wrong child coords
	// when redo happens and resizes when he doesn`t need it.
	// So we mush pre-translate child into new scene position first, but when
	// it lays in some container it also resizes. So we need to change parent to
	// root, then translate into a new position and change parent to a new one.
	// Also that element itself doesn`t change position in change parent command
	// so using translation command
	ChangeParentCommand *changeParentToSceneCommand =
			new ChangeParentCommand(mLogicalAssistApi, mGraphicalAssistApi, false
					, id(), oldParent, evScene->rootItemId(), oldPos, oldScenePos);
	AbstractCommand *translateCommand = ResizeCommand::create(this, mContents
			, position, mContents, oldScenePos);
	ChangeParentCommand *result = new ChangeParentCommand(
			mLogicalAssistApi, mGraphicalAssistApi, false
			, id(), evScene->rootItemId(), newParent, position, position);
	result->addPreAction(changeParentToSceneCommand);
	result->addPreAction(translateCommand);
	return result;
}

void NodeElement::updateShape(const QString &shape) const
{
	mElementImpl->updateRendererContent(shape);
}

IdList NodeElement::sortedChildren() const
{
	IdList result;
	if (mGraphicalAssistApi.properties(mId).contains("childrenOrder")) {
		for (const QString &id : mGraphicalAssistApi.graphicalRepoApi().property(mId, "childrenOrder")
				.toStringList()) {
			result << Id::loadFromString(id);
		}
	}

	return result;
}

void NodeElement::initRenderedDiagram()
{
	if (!mIsExpanded || mLogicalAssistApi.logicalRepoApi().outgoingExplosion(logicalId()) == Id()) {
		return;
	}

	EditorViewScene *evScene = dynamic_cast<EditorViewScene *>(scene());
	if (!evScene) {
		return;
	}

	const Id diagram = mLogicalAssistApi.logicalRepoApi().outgoingExplosion(logicalId());
	const Id graphicalDiagram = mGraphicalAssistApi.graphicalIdsByLogicalId(diagram)[0];

	EditorView view(evScene->models(), evScene->controller(), evScene->customizer(), graphicalDiagram);
	view.mutableScene().setNeedDrawGrid(false);

	view.mutableMvIface().configure(mGraphicalAssistApi, mLogicalAssistApi, mExploser);
	view.mutableMvIface().setModel(evScene->models().graphicalModel());
	view.mutableMvIface().setLogicalModel(evScene->models().logicalModel());
	view.mutableMvIface().setRootIndex(mGraphicalAssistApi.indexById(graphicalDiagram));

	QRectF sceneRect = view.editorViewScene().itemsBoundingRect();
	QImage image(sceneRect.size().toSize(), QImage::Format_RGB32);
	QPainter painter(&image);

	QBrush brush(Qt::SolidPattern);
	brush.setColor(Qt::white);
	painter.setBrush(brush);
	painter.setPen(QPen(Qt::white));
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::HighQualityAntialiasing);

	sceneRect.moveTo(QPointF());
	painter.drawRect(sceneRect);

	view.mutableScene().render(&painter);

	mRenderedDiagram = image;
}

QRectF NodeElement::diagramRenderingRect() const
{
	const NodeElement *initial = new NodeElement(
			mLogicalAssistApi.editorManagerInterface().elementImpl(id())
			, id().sameTypeId()
			, mModels
			);

	const qreal xCoeff = (boundingRect().width() - 3 * kvadratik) / (initial->boundingRect().width() - 3 * kvadratik);
	const qreal yCoeff = (boundingRect().height() - 3 * kvadratik) / (initial->boundingRect().height() - 3 *kvadratik);

	delete initial;

	// QReal:BP hardcode
	// TODO: Remove this.
	QRectF result(QPointF(25 * xCoeff, 25 * yCoeff), QPointF(185 * xCoeff, 115 * yCoeff));

	QPointF oldCenter(result.center());
	result.setSize(mRenderedDiagram.size().scaled(result.size().toSize(), Qt::KeepAspectRatio));
	result.moveCenter(oldCenter);
	return result;
}
