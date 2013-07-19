#include <QtCore/QUuid>
#include <QtWidgets/QStyle>
#include <QtWidgets/QStyleOptionGraphicsItem>
#include <QtWidgets/QMessageBox>
#include <QtGui/QTextCursor>
#include <QtWidgets/QToolTip>
#include <QtWidgets/QGraphicsDropShadowEffect>

#include <math.h>

#include "nodeElement.h"
#include "../view/editorViewScene.h"
#include "../editorPluginInterface/editorInterface.h"
#include "../mainwindow/mainWindow.h"

#include "private/resizeHandler.h"
#include "private/copyHandler.h"
#include "private/resizeCommand.h"
#include "private/foldCommand.h"

#include "../controller/commands/changeParentCommand.h"
#include "../controller/commands/insertIntoEdgeCommand.h"

using namespace qReal;
using namespace qReal::commands;

NodeElement::NodeElement(ElementImpl* impl)
	: Element(impl)
	, mSwitchGridAction(tr("Switch on grid"), this)
	, mPortsVisible(false)
	, mDragState(None)
	, mResizeCommand(NULL)
	, mIsExpanded(false)
	, mIsFolded(false)
	, mLeftPressed(false)
	, mParentNodeElement(NULL)
	, mPos(QPointF(0,0))
	, mSelectionNeeded(false)
	, mConnectionInProgress(false)
	, mPlaceholder(NULL)
	, mHighlightedNode(NULL)
	, mTimeOfUpdate(0)
	, mTimer(new QTimer(this))
{
	setAcceptHoverEvents(true);
	setFlag(ItemClipsChildrenToShape, false);
	setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren);

	mPortRenderer = new SdfRenderer();
	mRenderer = new SdfRenderer();
	ElementTitleFactory factory;
	QList<ElementTitleInterface*> titles;

	QList<StatPoint> pointPorts;
	QList<StatLine> linePorts;
	mElementImpl->init(mContents, pointPorts, linePorts, factory, titles, mRenderer, mPortRenderer, this);
	mPortHandler = new PortHandler(this, mGraphicalAssistApi, pointPorts, linePorts);

	foreach (ElementTitleInterface *titleIface, titles) {
		ElementTitle *title = dynamic_cast<ElementTitle*>(titleIface);
		if (!title) {
			continue;
		}
		title->init(mContents);
		title->setParentItem(this);
		mTitles.append(title);
	}

	mFoldedContents = mContents;

	mSwitchGridAction.setCheckable(true);
	connect(&mSwitchGridAction, SIGNAL(toggled(bool)), this, SLOT(switchGrid(bool)));

	foreach (QString bonusField, mElementImpl->bonusContextMenuFields()) {
		mBonusContextMenuActions.push_back(new ContextMenuAction(bonusField, this));
	}

	mGrid = new SceneGridHandler(this);
	mUmlPortHandler = new UmlPortHandler(this);
	switchGrid(SettingsManager::value("ActivateGrid").toBool());

	connect(mTimer, SIGNAL(timeout()), this, SLOT(updateNodeEdges()));
}

NodeElement::~NodeElement()
{
	highlightEdges();

	foreach (EdgeElement *edge, mEdgeList) {
		edge->removeLink(this);
	}

	foreach (ElementTitle *title, mTitles) {
		delete title;
	}

	delete mPortRenderer;
	delete mRenderer;
	delete mElementImpl;

	foreach (ContextMenuAction* action, mBonusContextMenuActions) {
		delete action;
	}

	delete mGrid;
	delete mUmlPortHandler;
	delete mPortHandler;
}

NodeElement *NodeElement::clone(bool toCursorPos, bool searchForParents)
{
	CopyHandler copyHandler(this, mGraphicalAssistApi);
	return copyHandler.clone(toCursorPos, searchForParents);
}

NodeElement* NodeElement::copyAndPlaceOnDiagram(QPointF const &offset)
{
	NodeElement* copy = clone(false, false);
	QPointF pos = copy->scenePos();
	copy->setPos(pos.x() + offset.x(), pos.y() + offset.y());

	return copy;
}

QMap<QString, QVariant> NodeElement::graphicalProperties() const
{
	return mGraphicalAssistApi->properties(id());
}

QMap<QString, QVariant> NodeElement::logicalProperties() const
{
	return mGraphicalAssistApi->properties(logicalId());
}

void NodeElement::setName(QString value)
{
	mGraphicalAssistApi->setName(id(), value);
}

void NodeElement::setGeometry(QRectF const &geom)
{
	prepareGeometryChange();
	setPos(geom.topLeft());
	if (geom.isValid()) {
		mContents = geom.translated(-geom.topLeft());
	}
	mTransform.reset();
	mTransform.scale(mContents.width(), mContents.height());
	adjustLinks();

	foreach (ElementTitle * const title, mTitles) {
		title->transform(geom);
	}

}

void NodeElement::setPos(QPointF const &pos)
{
	mPos = pos;
	QGraphicsItem::setPos(pos);
}

void NodeElement::setPos(qreal x, qreal y)
{
	setPos(QPointF(x, y));
}

void NodeElement::adjustLinks(bool isDragging)
{
	foreach (EdgeElement *edge, mEdgeList) {
		edge->adjustLink(isDragging);
	}

	foreach (QGraphicsItem *child, childItems()) {
		NodeElement *element = dynamic_cast<NodeElement*>(child);
		if (element) {
			element->adjustLinks(isDragging);
		}
	}
}

void NodeElement::arrangeLinearPorts()
{
	mPortHandler->arrangeLinearPorts();
}

void NodeElement::arrangeLinks()
{
	//Episode I: Home Jumps
	foreach (EdgeElement* edge, mEdgeList) {
		NodeElement* src = edge->src();
		NodeElement* dst = edge->dst();
		edge->reconnectToNearestPorts(this == src, this == dst);
	}

	//Episode II: Home Ports Arranging
	arrangeLinearPorts();

	//Episode III: Remote Jumps
	foreach (EdgeElement* edge, mEdgeList) {
		NodeElement* src = edge->src();
		NodeElement* dst = edge->dst();
		NodeElement* other = edge->otherSide(this);
		edge->reconnectToNearestPorts(other == src, other == dst);
	}

	//Episode IV: Remote Arrangigng
	QSet<NodeElement*> arranged;
	foreach (EdgeElement* edge, mEdgeList) {
		NodeElement* other = edge->otherSide(this);
		if (other && !arranged.contains(other)) {
			other->arrangeLinearPorts();
			arranged.insert(other);
		}
	}
}

void NodeElement::storeGeometry()
{
	QRectF contents = mContents; // saving correct current contents

	if ((pos() != mGraphicalAssistApi->position(id()))) { // check if it's been changed
		mGraphicalAssistApi->setPosition(id(), pos());
	}

	if (QPolygon(contents.toAlignedRect()) != mGraphicalAssistApi->configuration(id())) { // check if it's been changed
		mGraphicalAssistApi->setConfiguration(id(), QPolygon(contents.toAlignedRect()));
	}
}

QList<ContextMenuAction*> NodeElement::contextMenuActions(const QPointF &pos)
{
	Q_UNUSED(pos);
	QList<ContextMenuAction*> result;
	result.push_back(&mSwitchGridAction);
	foreach (ContextMenuAction* action, mBonusContextMenuActions) {
		result.push_back(action);
	}
	return result;
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

		// Align mode doesn`t work in a square mode
		if (SettingsManager::value("LineType").toInt() != static_cast<int>(squareLine)) {
			foreach (EdgeElement * const edge, mEdgeList) {
				edge->alignToGrid();
			}
		}
	}
}

void NodeElement::delUnusedLines()
{
	mGrid->delUnusedLines();
}

void NodeElement::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::RightButton) {
		event->accept();
		return;
	}

	mResizeCommand = new ResizeCommand(dynamic_cast<EditorViewScene *>(scene()), id());
	mResizeCommand->startTracking();
	if (isSelected()) {
		int dragArea = SettingsManager::instance()->value("DragArea").toInt();
		if (QRectF(mContents.topLeft(), QSizeF(dragArea, dragArea)).contains(event->pos()) && mElementImpl->isResizeable()) {
			mDragState = TopLeft;
		} else if (QRectF(mContents.topRight(), QSizeF(-dragArea, dragArea)).contains(event->pos()) && mElementImpl->isResizeable()) {
			mDragState = TopRight;
		} else if (QRectF(mContents.bottomRight(), QSizeF(-dragArea, -dragArea)).contains(event->pos()) && mElementImpl->isResizeable()) {
			mDragState = BottomRight;
		} else if (QRectF(mContents.bottomLeft(), QSizeF(dragArea, -dragArea)).contains(event->pos()) && mElementImpl->isResizeable()) {
			mDragState = BottomLeft;
		} else if (QRectF(QPointF(-20, 0), QPointF(0, 20)).contains(event->pos()) && mElementImpl->isContainer()) {
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

	if (!isPort() && (flags() & ItemIsMovable)) {
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

void NodeElement::recalculateHighlightedNode(QPointF const &mouseScenePos) {
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

	if (newParent != NULL) {
		if (mHighlightedNode) {
			mHighlightedNode->erasePlaceholder(false);
		}
		mHighlightedNode = newParent;
		mHighlightedNode->drawPlaceholder(EditorViewScene::getPlaceholder(), mouseScenePos);
	} else if (mHighlightedNode != NULL) {
		mHighlightedNode->erasePlaceholder(true);
		mHighlightedNode = NULL;
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
			qreal const size = qMax(newContents.width(), newContents.height());
			newContents.setWidth(size);
			newContents.setHeight(size);
		}
	}

	resize(newContents, newPos, needResizeParent);

	if (isPort()) {
		mUmlPortHandler->handleMoveEvent(mLeftPressed, mPos, event->scenePos(), mParentNodeElement);
	}

	// OMFG.
	if (mTimeOfUpdate == 14) {
		mTimeOfUpdate = 0;
		foreach (EdgeElement* edge, mEdgeList) {
			edge->adjustNeighborLinks();
		}
		arrangeLinks();
	} else {
		mTimeOfUpdate++;
	}
	mTimer->start(400);
}

void NodeElement::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	mTimer->stop();
	mTimeOfUpdate = 0;
	if (event->button() == Qt::RightButton) {
		event->accept();
		return;
	}
	delUnusedLines();

	storeGeometry();

	if (scene() && scene()->selectedItems().size() == 1 && isSelected()) {
		setVisibleEmbeddedLinkers(true);
	}

	if (mDragState == None) {
		Element::mouseReleaseEvent(event);
	}

	EditorViewScene *evScene = dynamic_cast<EditorViewScene *>(scene());
	commands::InsertIntoEdgeCommand *insertCommand = new commands::InsertIntoEdgeCommand(
			evScene, mLogicalAssistApi, mGraphicalAssistApi, id(), id(), Id::rootId()
			, event->scenePos(), boundingRect().bottomRight(), false);

	bool shouldProcessResize = true;

	// we should use mHighlightedNode to determine if there is a highlighted node
	// insert current element into them and set mHighlightedNode to NULL
	// but because of mouseRelease twice triggering we can't do it
	// This may cause more bugs
	if (!isPort() && (flags() & ItemIsMovable)) {
		if (mHighlightedNode) {
			NodeElement *newParent = mHighlightedNode;
			Element *insertBefore = mHighlightedNode->getPlaceholderNextElement();
			mHighlightedNode->erasePlaceholder(false);
			// commented because of bug with double event sending (see #204)
	//		mHighlightedNode = NULL;

			QPointF newPos = mapToItem(newParent, mapFromScene(scenePos()));
			AbstractCommand *parentCommand = changeParentCommand(newParent->id(), newPos);
			mController->execute(parentCommand);
			// Position change already processed in change parent command
			shouldProcessResize = parentCommand == NULL;
			setPos(newPos);

			if (insertBefore) {
				mGraphicalAssistApi->stackBefore(id(), insertBefore->id());
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
			shouldProcessResize = parentCommand == NULL;
		}
	}
	if (shouldProcessResize && mResizeCommand) {
		mResizeCommand->stopTracking();
		if (mResizeCommand->modificationsHappened()) {
			mResizeCommand->addPostAction(insertCommand);
			mController->execute(mResizeCommand);
		} else {
			delete mResizeCommand;
		}
		// Undo stack took ownership
		mResizeCommand = NULL;
	}

	arrangeLinks();

	foreach (EdgeElement* edge, mEdgeList) {
		edge->adjustNeighborLinks();
	}
	adjustLinks();
	foreach (EdgeElement* edge, mEdgeList) {
		edge->setGraphicApiPos();
		edge->saveConfiguration(QPointF());
		if (SettingsManager::value("ActivateGrid").toBool())
		{
			edge->alignToGrid();
		}
	}

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
}

void NodeElement::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	Q_UNUSED(event);
}

void NodeElement::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	Q_UNUSED(event);
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

	foreach (QString const &elementName, mGraphicalAssistApi->editorManagerInterface().elements(id().editor(),id().diagram())) {
		int ne = mGraphicalAssistApi->editorManagerInterface().isNodeOrEdge(id().editor(), elementName);
		if (ne == -1) {
			QList<StringPossibleEdge> const list =  mGraphicalAssistApi->editorManagerInterface().possibleEdges(id().editor(), elementName);
			foreach(StringPossibleEdge pEdge, list) {
				if (mGraphicalAssistApi->editorManagerInterface().isParentOf(id().editor(), id().diagram()
						, pEdge.first.first, id().diagram(), id().element())
						|| (mGraphicalAssistApi->editorManagerInterface().isParentOf(id().editor(), id().diagram()
						, pEdge.first.second, id().diagram(), id().element()) && !pEdge.second.first))
				{
					PossibleEdge possibleEdge = toPossibleEdge(pEdge);
					mPossibleEdges.insert(possibleEdge);
					mPossibleEdgeTypes.insert(possibleEdge.second);
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
	QSet<qReal::Id> usedEdges;
	foreach (PossibleEdgeType type, mPossibleEdgeTypes) {
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
	// foreach (EmbeddedLinker* embeddedLinker, mEmbeddedLinkers) {
	// embeddedLinker->initTitle();
	// }
}

void NodeElement::setVisibleEmbeddedLinkers(bool const show)
{
	if (show) {
		setZValue(250);
		int index = 0;
		int maxIndex = mEmbeddedLinkers.size();
		foreach (EmbeddedLinker* embeddedLinker, mEmbeddedLinkers) {
			embeddedLinker->takePosition(index,maxIndex);
			embeddedLinker->show();
			index++;
		}
	} else {
		setZValue(0);
		foreach (EmbeddedLinker* embeddedLinker, mEmbeddedLinkers) {
			embeddedLinker->hide();
		}
	}
}

QVariant NodeElement::itemChange(GraphicsItemChange change, QVariant const &value)
{
	bool isItemAddedOrDeleted = false;
	NodeElement *item = dynamic_cast<NodeElement*>(value.value<QGraphicsItem*>());
	switch (change) {
	case ItemPositionHasChanged:
		if (mDragState == None) {
			alignToGrid();
		}
		adjustLinks(true);
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
		QPointF newpos = mGraphicalAssistApi->position(id());
		QPolygon newpoly = mGraphicalAssistApi->configuration(id());

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
	update();
}

QPointF const NodeElement::portPos(qreal id) const
{
	return mPortHandler->portPos(id);
}

QPointF const NodeElement::nearestPort(QPointF const &location) const
{
	return mPortHandler->nearestPort(location);
}

bool NodeElement::isContainer() const
{
	return mElementImpl->isContainer();
}

int NodeElement::numberOfPorts() const
{
	return mPortHandler->numberOfPorts();
}

int NodeElement::portNumber(qreal id)
{
	return PortHandler::portNumber(id);
}

qreal NodeElement::portId(QPointF const &location) const
{
	return mPortHandler->portId(location);
}

void NodeElement::setPortsVisible(bool value)
{
	prepareGeometryChange();
	mPortsVisible = value;
}

NodeElement *NodeElement::getNodeAt(QPointF const &position)
{
	foreach (QGraphicsItem *item, scene()->items(position)) {
		NodeElement *e = dynamic_cast<NodeElement *>(item);
		if (e && (item != this)) {
			return e;
		}
	}
	return 0;
}

void NodeElement::paint(QPainter *painter, QStyleOptionGraphicsItem const *style, QWidget *w)
{
	mElementImpl->paint(painter, mContents);
	if (mElementImpl->hasPorts()) {
		paint(painter, style, w, mPortRenderer);
	} else {
		paint(painter, style, w, 0);
	}

	if (mSelectionNeeded) {
		painter->save();
		painter->setPen(QPen(Qt::blue));
		QRectF rect = boundingRect();
		double x1 = rect.x() + 9;
		double y1 = rect.y() + 9;
		double x2 = rect.x() + rect.width() - 9;
		double y2 = rect.y() + rect.height() - 9;
		painter->drawRect(QRectF(QPointF(x1, y1), QPointF(x2, y2)));
		painter->restore();
	}
}

void NodeElement::paint(QPainter *painter, QStyleOptionGraphicsItem const *option
		, QWidget *, SdfRenderer* portRenderer)
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
			painter->save();

			if (mElementImpl->isResizeable()) {
				drawLinesForResize(painter);
			} else {
				painter->drawRect(QRectF(mContents.bottomRight(), QSizeF(-4, -4)));
			}

			painter->restore();
		}
		if (((option->state & QStyle::State_MouseOver) || mPortsVisible) && portRenderer) {
			painter->save();
			painter->setOpacity(0.7);
			portRenderer->render(painter, mContents);
			painter->restore();
		}

		if (mIsExpanded && !mLogicalAssistApi->logicalRepoApi().outgoingConnections(logicalId()).empty()) {
			initRenderedDiagram();
			painter->drawImage(diagramRenderingRect(), mRenderedDiagram);
		}
	}
}

QList<EdgeElement*> NodeElement::getEdges()
{
	return mEdgeList;
}

void NodeElement::addEdge(EdgeElement *edge)
{
	mEdgeList << edge;
}

void NodeElement::delEdge(EdgeElement *edge)
{
	mEdgeList.removeAll(edge);
}

void NodeElement::changeExpanded()
{
	mIsExpanded = !mIsExpanded;
}

void NodeElement::changeFoldState()
{
	mIsFolded = !mIsFolded;

	foreach (QGraphicsItem* childItem, childItems()) {
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

	NodeElement* parent = dynamic_cast<NodeElement*>(parentItem());
	if (parent) {
		parent->resize();
	}
}

void NodeElement::setLinksVisible(bool isVisible)
{
	foreach (EdgeElement *curEdge, mEdgeList) {
		curEdge->setVisible(isVisible);
	}

	foreach (QGraphicsItem* childItem, childItems()) {
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
	NodeElement *nextItem = NULL;

	foreach (QGraphicsItem* childItem, childItems()) {
		NodeElement *curItem = dynamic_cast<NodeElement*>(childItem);
		if (curItem != NULL) {
			if (curItem->scenePos().y() > pos.y()) {
				nextItem = curItem;
				break;
			}
		}
	}

	erasePlaceholder(false);
	mPlaceholder = placeholder;
	mPlaceholder->setParentItem(this);
	if(nextItem != NULL) {
		mPlaceholder->stackBefore(nextItem);
	}

	resize();
}

Element* NodeElement::getPlaceholderNextElement()
{
	if(mPlaceholder == NULL) {
		return NULL;
	}
	bool found = false;
	// loking for child following the placeholder
	foreach(QGraphicsItem *childItem, childItems()) {
		Element *element = dynamic_cast<Element*>(childItem);
		if(found && element != NULL) {
			return element;
		}
		if(childItem == mPlaceholder) {
			found = true;
		}
	}
	return NULL;
}

void NodeElement::erasePlaceholder(bool redraw)
{
	setOpacity(1);

	if (!mPlaceholder) {
		return;
	}

	delete mPlaceholder;
	mPlaceholder = NULL;

	if(redraw) {
		resize();
	}
}

bool NodeElement::isPort() const
{
	return mElementImpl->isPort();
}

bool NodeElement::canHavePorts()
{
	return mElementImpl->hasPin();
}

void NodeElement::updateByChild(NodeElement* item, bool isItemAddedOrDeleted)
{
	if (mIsFolded && isItemAddedOrDeleted && item) {
		changeFoldState();
	}

	resize();
}

void NodeElement::updateByNewParent()
{
	EditorViewScene *editorScene = dynamic_cast<EditorViewScene *>(scene());
	if (editorScene) {
		editorScene->onElementParentChanged(this);
	}
	NodeElement* parent = dynamic_cast<NodeElement*>(parentItem());
	if (!parent || parent->mElementImpl->hasMovableChildren()) {
		setFlag(ItemIsMovable, true);
	} else {
		setFlag(ItemIsMovable, false);
	}
}

QList<double> NodeElement::borderValues() const
{
	return mElementImpl->border();
}

PossibleEdge NodeElement::toPossibleEdge(StringPossibleEdge const &strPossibleEdge)
{
	QString editor = id().editor();
	QString diagram = id().diagram();
	QPair<qReal::Id, qReal::Id> nodes(qReal::Id(editor, diagram, strPossibleEdge.first.first),
									  qReal::Id(editor, diagram, strPossibleEdge.first.second));
	QPair<bool, qReal::Id> link(strPossibleEdge.second.first,
								qReal::Id(editor, diagram, strPossibleEdge.second.second));
	return QPair<QPair<qReal::Id, qReal::Id>, PossibleEdgeType>(nodes, link);
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

void NodeElement::connectLinksToPorts()
{
	mPortHandler->connectLinksToPorts();
}

void NodeElement::singleSelectionState(bool const singleSelected)
{
	initEmbeddedLinkers();
	setVisibleEmbeddedLinkers(singleSelected);
	setTitlesVisiblePrivate(singleSelected || mTitlesVisible);
	Element::singleSelectionState(singleSelected);
}

void NodeElement::selectionState(bool const selected)
{
	Element::selectionState(selected);
}

void NodeElement::highlightEdges()
{
	foreach (EdgeElement *edge, mEdgeList) {
		edge->highlight();
	}
}

void NodeElement::disconnectEdges()
{
	foreach (EdgeElement *edge, mEdgeList) {
		if (edge->src() == this) {
			mGraphicalAssistApi->setFrom(edge->id(), Id::rootId());
			mLogicalAssistApi->setFrom(edge->logicalId(), Id::rootId());
		}

		if (edge->dst() == this) {
			mGraphicalAssistApi->setTo(edge->id(), Id::rootId());
			mLogicalAssistApi->setTo(edge->logicalId(), Id::rootId());
		}

		edge->removeLink(this);
	}
	mEdgeList.clear();
}

void NodeElement::deleteFromScene()
{
	highlightEdges();
	disconnectEdges();
}

NodeData& NodeElement::data()
{
	mData.id = id();
	mData.logicalId = logicalId();
	mData.logicalProperties = logicalProperties();
	mData.graphicalProperties = graphicalProperties();
	// new element should not have references to links connected to original source element
	mData.graphicalProperties["links"] = IdListHelper::toVariant(IdList());
	mData.pos = mPos;
	mData.contents = mContents;

	NodeElement *parent = dynamic_cast<NodeElement *>(parentItem());
	if (parent) {
		mData.parentId = parent->id();
	} else {
		mData.parentId = Id::rootId();
	}

	return mData;
}

void NodeElement::resize()
{
	resize(mContents, pos());
}

void NodeElement::resize(QRectF const &newContents)
{
	resize(newContents, pos());
}

void NodeElement::resize(QRectF const &newContents, QPointF const &newPos, bool needResizeParent)
{
	ResizeHandler handler(this);
	handler.resize(newContents, newPos, needResizeParent);
}

void NodeElement::drawLinesForResize(QPainter *painter)
{
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
	foreach (QGraphicsItem *item, childItems()) {
		NodeElement *child = dynamic_cast<NodeElement *>(item);
		if (child) {
			result << child;
		}
	}
	return result;
}

void NodeElement::setAssistApi(qReal::models::GraphicalModelAssistApi *graphicalAssistApi, qReal::models::LogicalModelAssistApi *logicalAssistApi)
{
	Element::setAssistApi(graphicalAssistApi, logicalAssistApi);
	mPortHandler->setGraphicalAssistApi(graphicalAssistApi);
}


void NodeElement::updateNodeEdges()
{
	mTimer->stop();
	mTimeOfUpdate = 0;
	arrangeLinks();
	foreach (EdgeElement* edge, mEdgeList) {
		edge->adjustNeighborLinks();
	}
}

AbstractCommand *NodeElement::changeParentCommand(Id const &newParent, QPointF const &position) const
{
	EditorViewScene *evScene = dynamic_cast<EditorViewScene *>(scene());
	Element *oldParentElem = dynamic_cast<Element *>(parentItem());
	Id const oldParent = oldParentElem ? oldParentElem->id() : evScene->rootItemId();
	if (oldParent == newParent) {
		return NULL;
	}
	QPointF const oldPos = mResizeCommand ? mResizeCommand->geometryBeforeDrag().topLeft() : mPos;
	QPointF const oldScenePos = oldParentElem ? oldParentElem->mapToScene(oldPos) : oldPos;
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

void NodeElement::updateShape(QString const &shape) const
{
	mElementImpl->updateRendererContent(shape);
}

bool NodeElement::operator<(NodeElement const &other) const
{
	return y() < other.y();
}

void NodeElement::initRenderedDiagram()
{
	IdList diagrams = mLogicalAssistApi->logicalRepoApi().outgoingConnections(logicalId());
	if (diagrams.empty()) {
		return;
	}

	EditorViewScene *evScene = dynamic_cast<EditorViewScene *>(scene());
	MainWindow *window = evScene->mainWindow();

	Id diagram = mLogicalAssistApi->logicalRepoApi().outgoingConnections(logicalId())[0];
	Id graphicalDiagram = mGraphicalAssistApi->graphicalIdsByLogicalId(diagram)[0];

	EditorView * view = new EditorView(window);
	EditorViewScene *openedScene = dynamic_cast<EditorViewScene *>(view->scene());
	openedScene->setMainWindow(window);
	openedScene->setNeedDrawGrid(false);

	view->mvIface()->setAssistApi(window->models()->graphicalModelAssistApi()
			, window->models()->logicalModelAssistApi());
	view->mvIface()->setModel(window->models()->graphicalModel());
	view->mvIface()->setLogicalModel(window->models()->logicalModel());
	view->mvIface()->setRootIndex(window->models()->graphicalModelAssistApi().indexById(graphicalDiagram));

	QRectF sceneRect = openedScene->itemsBoundingRect();
	QImage image(sceneRect.size().toSize(), QImage::Format_RGB32);
	QPainter painter(&image);

	QBrush brush(Qt::SolidPattern);
	brush.setColor(Qt::white);
	painter.setBrush(brush);
	painter.setPen(QPen(Qt::white));

	sceneRect.moveTo(QPointF());
	painter.drawRect(sceneRect);

	openedScene->render(&painter);

	delete view;
	mRenderedDiagram = image;
}

QRectF NodeElement::diagramRenderingRect() const
{
	EditorViewScene *evScene = dynamic_cast<EditorViewScene *>(scene());
	NodeElement *initial = dynamic_cast<NodeElement *>(evScene->mainWindow()->editorManager().graphicalObject(id()));
	qreal xCoeff = boundingRect().width() / initial->boundingRect().width();
	qreal yCoeff = boundingRect().height() / initial->boundingRect().height();

	// QReal:BP hardcode
	QRectF result(QPointF(30 * xCoeff, 30 * yCoeff), QPointF(195 * xCoeff, 120 * yCoeff));

	qreal renderedDiagramRatio = mRenderedDiagram.width() / mRenderedDiagram.height();
	QPointF oldCenter(result.center());
	if (renderedDiagramRatio > 1) {
		result.setHeight(result.width() / renderedDiagramRatio);
	} else {
		result.setWidth(result.height() * renderedDiagramRatio);
	}
	result.moveCenter(oldCenter);

	return result;
}
