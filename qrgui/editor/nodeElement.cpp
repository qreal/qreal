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
#include <qrutils/scalableItem.h>

#include <qrgui/models/models.h>
#include <qrgui/models/commands/changeParentCommand.h>
#include <qrgui/models/commands/renameCommand.h>
#include <metaMetaModel/edgeElementType.h>
#include <metaMetaModel/nodeElementType.h>

#include "editor/labels/label.h"
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

NodeElement::NodeElement(const NodeElementType &type, const Id &id, const models::Models &models)
	: Element(type, id, models)
	, mType(type)
	, mSwitchGridAction(tr("Switch on grid"), this)
	, mContents(QPointF(), type.size())
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

	mRenderer.load(mType.sdf());
	mRenderer.setElementRepo(this);

	PortFactory portFactory;
	mPortHandler = new PortHandler(this, mGraphicalAssistApi
			, portFactory.createPorts(mType.pointPorts())
			, portFactory.createPorts(mType.linePorts())
			, portFactory.createPorts(mType.circularPorts()));

	const QList<LabelProperties> labelInfos = mType.labels();
	for (const LabelProperties &labelInfo : labelInfos) {
		Label * const label = new Label(mGraphicalAssistApi, mLogicalAssistApi, mId, labelInfo);
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

	mStartingLabelsCount = mLabels.count();

	initExplosionConnections();
	const Id explosionTarget = mLogicalAssistApi.logicalRepoApi().outgoingExplosion(mGraphicalAssistApi.logicalId(mId));
	if (!explosionTarget.isNull()) {
		models.exploser().explosionTargetCouldChangeProperties(explosionTarget);
	}
}

NodeElement::~NodeElement()
{
	for (EdgeElement * const edge : mEdgeList) {
		edge->removeLink(this);
	}

	deleteGuides();
	qDeleteAll(mLabels);
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

void NodeElement::initExplosionConnections()
{
	connect(&mModels.exploser(), &models::Exploser::explosionTargetCouldChangeProperties, this
			, &NodeElement::updateDynamicProperties);
}

void NodeElement::updateDynamicProperties(const Id &target)
{
	const Id outgoingExplosion = mLogicalAssistApi.logicalRepoApi().outgoingExplosion(logicalId());
	if (outgoingExplosion.isNull()) {
		// remove this line if there are language elements which can add explosion
		disconnect(&mModels.exploser(), &models::Exploser::explosionTargetCouldChangeProperties, this
				, &NodeElement::updateDynamicProperties);
		return;
	}

	if (outgoingExplosion != target) {
		return;
	}

	// Update name
	const QString name = mLogicalAssistApi.mutableLogicalRepoApi().stringProperty(target, "name");
	if (mLogicalAssistApi.mutableLogicalRepoApi().stringProperty(logicalId(), "name") != name) {
		setName(name, false);
	}

	// Update shape
	const QString shape = mLogicalAssistApi.mutableLogicalRepoApi().stringProperty(target, "shape");
	if (mPreviousShape != shape) {
		mPreviousShape = shape;
		if (shape.isEmpty()) {
			mRenderer.load(mType.sdf().ownerDocument());
		} else {
			QDomDocument picture;
			picture.setContent(shape);
			mRenderer.load(picture);
		}

		update();
	}

	// Get labels
	const QString dynamiclabels = mLogicalAssistApi.mutableLogicalRepoApi().stringProperty(target, "labels");
	if (mPreviousDynamicLabels.isEmpty() ||
			(!mPreviousDynamicLabels.isEmpty() && compareDynamicLabels(dynamiclabels, mPreviousDynamicLabels))) {
		mPreviousDynamicLabels = dynamiclabels;
		QDomDocument dynamicProperties;
		QDomElement properties = dynamicProperties.createElement("properties");
		QDomDocument dynamicLabels;
		dynamicLabels.setContent(dynamiclabels);

		// ...delete old dynamic labels
		const int oldCount = mLabels.count() - mStartingLabelsCount;
		for (int i = 0; i < oldCount; ++i) {
			delete mLabels.takeLast();
		}

		// if we restore block from repo, we already have properties all properties Value
		const QString mCurretDynamicProperties
				= mLogicalAssistApi.mutableLogicalRepoApi().stringProperty(logicalId(), "dynamicProperties");
		QMap<QString, QString> valueByPropertyName;
		if (!mCurretDynamicProperties.isEmpty()) {
			QDomDocument dynamicProperties;
			dynamicProperties.setContent(mCurretDynamicProperties);

			for (QDomElement element
					= dynamicProperties.firstChildElement("properties").firstChildElement("property")
					; !element.isNull()
					; element = element.nextSiblingElement("property"))
			{
				valueByPropertyName[element.attribute("name")] = element.attribute("dynamicPropertyValue");
			}
		}

		int index = mLabels.count() + 1;
		for (QDomElement element = dynamicLabels.firstChildElement("labels").firstChildElement("label")
				; !element.isNull()
				; element = element.nextSiblingElement("label"), ++index)
		{
			utils::ScalableCoordinate x = utils::ScalableItem::initCoordinate(element.attribute("x")
					, mContents.width());
			utils::ScalableCoordinate y = utils::ScalableItem::initCoordinate(element.attribute("y")
					, mContents.height());
			const QString textBinded = element.attribute("textBinded");
			const QString value = valueByPropertyName.contains(textBinded)
					? valueByPropertyName[textBinded]
					: element.attribute("value");
			const QString type = element.attribute("type");
			const QString text = element.attribute("text");

			// Saving dynamicProperty
			// WARNING property should be saved before label initialization
			QDomElement property = dynamicProperties.createElement("property");
			property.setAttribute("name", textBinded);
			property.setAttribute("displayedName", text);
			property.setAttribute("type", type);
			property.setAttribute("dynamicPropertyValue", value);
			properties.appendChild(property);

			if (dynamicProperties.appendChild(properties).isNull()) {
				dynamicProperties.replaceChild(properties, properties);
			}

			mLogicalAssistApi.mutableLogicalRepoApi().setProperty(logicalId(), "dynamicProperties"
					, dynamicProperties.toString(4));

			// Label initialization
			LabelProperties labelInfo(index, x.value(), y.value(), textBinded, false, 0);
			labelInfo.setBackground(Qt::white);
			labelInfo.setScalingX(false);
			labelInfo.setScalingY(false);
			labelInfo.setHard(false);
			labelInfo.setPrefix(text);
			labelInfo.setPlainTextMode(true);
			Label *label = new Label(mGraphicalAssistApi, mLogicalAssistApi, mId, labelInfo);
			label->init(mContents);
			label->setParentItem(this);
			label->setTextInteractionFlags(Qt::TextEditorInteraction);
			label->setPlainText(value);
			mLabels.append(label);
		}
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
				&& mType.isResizable())
		{
			mDragState = TopLeft;
		} else if (QRectF(mContents.topRight(), QSizeF(-dragArea, dragArea)).contains(event->pos())
				&& mType.isResizable())
		{
			mDragState = TopRight;
		} else if (QRectF(mContents.bottomRight(), QSizeF(-dragArea, -dragArea)).contains(event->pos())
				&& mType.isResizable())
		{
			mDragState = BottomRight;
		} else if (QRectF(mContents.bottomLeft(), QSizeF(dragArea, -dragArea)).contains(event->pos())
				&& mType.isResizable())
		{
			mDragState = BottomLeft;
		} else if (QRectF(QPointF(-20, 0), QPointF(0, 20)).contains(event->pos())
				&& mType.isContainer())
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
		if (!parent || !parent->mType.isSortingContainer()) {
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
		newPos = pos();
	} else if (mType.isResizable()) {
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
			newPos = QPointF(pos().x(), event->scenePos().y() - parentPos.y());
			break;
		}
		case TopRight: {
			newContents.setTopRight(QPoint(newX, event->pos().y() - event->lastPos().y()));
			newPos = QPointF(newPos.x(), event->scenePos().y() - parentPos.y());
			break;
		}
		case Left: {
			newContents.setLeft(newX);
			newPos = QPointF(event->scenePos().x() - parentPos.x(), pos().y());
			break;
		}
		case Right: {
			newContents.setRight(newX);
			break;
		}
		case BottomLeft: {
			newContents.setBottomLeft(QPoint(event->pos().x() - event->lastPos().x(), newY));
			newPos = QPointF(event->scenePos().x() - parentPos.x(), pos().y());
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

	if (mType.isContainer()) {
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

void NodeElement::initEmbeddedLinkers()
{
	if (!mEmbeddedLinkers.isEmpty()) {
		return;
	}

	const IdList elements = mGraphicalAssistApi.editorManagerInterface().elements(id());
	for (const Id &element : elements) {
		const ElementType &elementType = mGraphicalAssistApi.editorManagerInterface().elementType(element);
		if (elementType.type() != ElementType::Type::edge) {
			continue;
		}

		const EdgeElementType &edge = elementType.toEdge();
		if (!edge.fromPortTypes().toSet().intersect(mType.portTypes().toSet()).isEmpty()) {
			EmbeddedLinker* embeddedLinker = new EmbeddedLinker();
			scene()->addItem(embeddedLinker);
			embeddedLinker->setEdgeType(edge.typeId());
			embeddedLinker->setDirected(true);
			mEmbeddedLinkers.append(embeddedLinker);
			embeddedLinker->setMaster(this);
		}
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

	updateLabels();
	updateDynamicLabels();
	update();
}

const QPointF NodeElement::portPos(qreal id) const
{
	return mPortHandler->portPos(id);
}

bool NodeElement::isContainer() const
{
	return mType.isContainer();
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
	mRenderer.render(painter, mContents);
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

			if (mType.isContainer()) {
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

			if (mType.isResizable()) {
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

	const QStringList mPortsVisibilityKeys = mPortsVisibility.keys(true);
	const QStringList portTypes = mouseOver ? mGraphicalAssistApi.editorManagerInterface().portTypes(id().type())
			: mPortsVisibilityKeys;
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

const NodeElementType &NodeElement::nodeType() const
{
	return mType;
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
	for (Label * const label : mLabels) {
		label->setParentContents(mContents);
	}
}

void NodeElement::updateDynamicLabels()
{
	for (Label *label : mLabels) {
		label->updateDynamicData();
	}
}

bool NodeElement::compareDynamicLabels(QString labelsPack1, QString labelsPack2) const
{
	QDomDocument dynamicLabels1;
	dynamicLabels1.setContent(labelsPack1);
	QDomDocument dynamicLabels2;
	dynamicLabels2.setContent(labelsPack2);
	QSet<QString> dynamicLabelsNames1;
	QSet<QString> dynamicLabelsNames2;

	auto traverse = [&](const QDomDocument &dynamicLabels, QSet<QString> &labelNamesSet) {
		for (QDomElement element = dynamicLabels.firstChildElement("labels").firstChildElement("label")
				; !element.isNull()
				; element = element.nextSiblingElement("label"))
		{
			const QString name = element.attribute("textBinded");
			labelNamesSet.insert(name);
		}
	};

	traverse(dynamicLabels1, dynamicLabelsNames1);
	traverse(dynamicLabels2, dynamicLabelsNames2);
	return dynamicLabelsNames1 != dynamicLabelsNames2;
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
	if (!mType.isSortingContainer()) {
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

	if (mType.isSortingContainer()) {
		updateChildrenOrder();
	}

	resize();
}

void NodeElement::updateByNewParent()
{
	NodeElement* parent = dynamic_cast<NodeElement*>(parentItem());
	if (!parent || parent->mType.hasMovableChildren()) {
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
	return mType.border();
}

void NodeElement::setColorRect(bool value)
{
	mSelectionNeeded = value;
}

void NodeElement::checkConnectionsToPort() // it is strange method
{
	mPortHandler->checkConnectionsToPort();
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
	ResizeHandler handler(*this);
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

void NodeElement::updateShape(const QDomElement &graphicsSdf)
{
	mRenderer.load(graphicsSdf);
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

	EditorView view(evScene->models(), evScene->controller(), evScene->sceneCustomizer(), graphicalDiagram);
	view.mutableScene().setNeedDrawGrid(false);

	view.mutableMvIface().configure(mGraphicalAssistApi, mLogicalAssistApi, mModels.exploser());
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
			mLogicalAssistApi.editorManagerInterface().elementType(id()).toNode()
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
