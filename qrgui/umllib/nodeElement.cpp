#include "nodeElement.h"
#include "../view/editorViewScene.h"
#include "../editorPluginInterface/editorInterface.h"

#include <QtGui/QStyle>
#include <QtGui/QStyleOptionGraphicsItem>
#include <QtGui/QMessageBox>
#include <QtGui/QTextCursor>
#include <QtGui/QToolTip>
#include <QtCore/QDebug>
#include <QtCore/QUuid>

#include <QtGui/QGraphicsDropShadowEffect>

#include <math.h>

using namespace qReal;

NodeElement::NodeElement(ElementImpl* impl)
		: mSwitchGridAction(tr("Switch on grid"), this)
		, mPortsVisible(false), mDragState(None), mElementImpl(impl), mIsFolded(false)
		, mLeftPressed(false), mParentNodeElement(NULL), mPos(QPointF(0,0))
		, mSelectionNeeded(false), mConnectionInProgress(false), mHighlightedNode(NULL)
{
	setAcceptHoverEvents(true);
	setFlag(ItemClipsChildrenToShape, false);
	setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren);

	mPortRenderer = new SdfRenderer();
	mRenderer = new SdfRenderer();
	ElementTitleFactory factory;
	QList<ElementTitleInterface*> titles;
	mElementImpl->init(mContents, mPointPorts, mLinePorts, factory, titles, mRenderer, mPortRenderer);
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
	switchGrid(SettingsManager::value("ActivateGrid", true).toBool());
}

NodeElement::~NodeElement()
{
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
}

NodeElement *NodeElement::clone(bool toCursorPos, bool shareLogicalId, Id const &parentId)
{
	NodeElement *result = NULL;
	EditorViewScene *evscene = dynamic_cast<EditorViewScene*>(scene());

	QPointF const placePos = toCursorPos ? evscene->getMousePos() : mPos;
	if (shareLogicalId) {
		qReal::Id resultId = mGraphicalAssistApi->createElement(parentId, logicalId(), true, mGraphicalAssistApi->name(id()), placePos);
		Element *element = evscene->mainWindow()->manager()->graphicalObject(resultId);
		result = dynamic_cast<NodeElement*>(element);
		result->setAssistApi(mGraphicalAssistApi, mLogicalAssistApi);
		result->setId(resultId);
		result->setPos(placePos);

		if (parentId == Id::rootId())
			scene()->addItem(result);
		else {
			Element *parent = evscene->getElem(parentId);
			result->setParentItem(parent);
		}

		result->updateData();
		result->connectToPort();
		result->checkConnectionsToPort();
		result->initPossibleEdges();
		result->initTitles();

	} else {
		qReal::Id typeId = id().type();
		qReal::Id *resultId = evscene->createElement(typeId.toString(), QPointF());

		result = dynamic_cast<NodeElement*>(evscene->getElem(*resultId));
	}

	Q_ASSERT(result != NULL);

	result->copyProperties(this);
	result->copyChildren(this, shareLogicalId);

	result->mContents = mContents;
	result->setPos(placePos);
	result->storeGeometry();

	return result;
}

void NodeElement::copyAndPlaceOnDiagram(bool shareLogicalId)
{
	clone(true, shareLogicalId);
}

void NodeElement::copyChildren(NodeElement *source, bool shareLogicalId)
{
	foreach (QGraphicsItem *child, source->childItems()) {
		NodeElement *element = dynamic_cast<NodeElement*>(child);
		if (element) {
			NodeElement *copyOfChild = element->clone(false, shareLogicalId, id());
			mGraphicalAssistApi->changeParent(copyOfChild->id(), id(), element->pos());
		}
	}
}

void NodeElement::copyProperties(NodeElement *source)
{
	mGraphicalAssistApi->copyProperties(id(), source->id());
}

NodeElementSerializationData NodeElement::serializationData() const
{
	NodeElementSerializationData data;
	data.mId = id();
	data.mLogicalId = logicalId();
	data.mProperties = mGraphicalAssistApi->properties(id());

	NodeElement *parent = dynamic_cast<NodeElement*>(parentItem());
	if (parent) {
		data.mParentId = parent->id();
	} else {
		data.mParentId = Id::rootId();
	}

	data.mPos = mPos;
	data.mContenets = mContents;

	return data;
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

void NodeElement::setContents(const QRectF &contents)
{
	mContents = contents;
}

void NodeElement::adjustLinks()
{
	foreach (EdgeElement *edge, mEdgeList) {
		edge->adjustLink();
	}

	foreach (QGraphicsItem *child, childItems()) {
		NodeElement *element = dynamic_cast<NodeElement*>(child);
		if (element) {
			element->adjustLinks();
		}
	}
}

// TODO: Understand what happens here ASAP!
/*
void NodeElement::arrangeLinks() {
	if (!SettingsManager::value("arrangeLinks", true).toBool()) {
		return;
	}

	QSet<NodeElement*> toArrange;
	QSet<NodeElement*> arranged;
	arrangeLinksRecursively(toArrange, arranged);

	foreach (QGraphicsItem *child, childItems()) {
		NodeElement *element = dynamic_cast<NodeElement*>(child);
		if (element) {
			element->arrangeLinks();
		}
	}
}
*/

/*
void NodeElement::arrangeLinksRecursively(QSet<NodeElement*>& toArrange, QSet<NodeElement*>& arranged)
{
	toArrange.remove(this);

	foreach (EdgeElement* edge, mEdgeList) {
		NodeElement* src = edge->src();
		NodeElement* dst = edge->dst();
		edge->reconnectToNearestPorts(this == src || !arranged.contains(src), this == dst || !arranged.contains(dst), false);
		NodeElement* other = edge->otherSide(this);
		if (!arranged.contains(other) && other != 0) {
			toArrange.insert(other);
		}
	}

	//make equal space on all linear ports.
	int lpId = 0;
	foreach (StatLine line, mLinePorts) {
		//sort first by slope, then by current portId
		QMap<QPair<qreal, qreal>, EdgeElement*> sortedEdges;
		QLineF portLine = line;
		qreal dx = portLine.dx();
		qreal dy = portLine.dy();
		foreach (EdgeElement* edge, mEdgeList) {
			if (portId(edge->portIdOn(this)) == lpId) {
				QPointF conn = edge->connectionPoint(this);
				QPointF next = edge->nextFrom(this);
				qreal x1 = conn.x();
				qreal y1 = conn.y();
				qreal x2 = next.x();
				qreal y2 = next.y();
				qreal len = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
				qreal scalarProduct = ((x2 - x1) * dx + (y2 - y1) * dy) / len;
				sortedEdges.insertMulti(qMakePair(edge->portIdOn(this), scalarProduct), edge);
				//qDebug() << "+" << edge->uuid().toString() <<"pr=" <<scalarProduct << "; p=" << edge->portIdOn(this);
				//qDebug("'--> vector: (%g, %g)", (x2-x1)/len, (y2-y1)/len);
				//qDebug() << "'------> because " << (QVariant)conn << "->" << (QVariant)next;
			}
		}

		//by now, edges of this port are sorted by their optimal slope.
		int N = sortedEdges.size();
		int i = 0;
		foreach (EdgeElement* edge, sortedEdges) {
			qreal newId = lpId + (1.0 + i++) / (N + 1);
			//qDebug() << "-" << edge->uuid().toString() << newId;
			edge->moveConnection(this, newId);
		}

		lpId++; //next linear port.

	}
}
*/

void NodeElement::arrangeLinearPorts() {
	int lpId = mPointPorts.size(); //point ports before linear
	foreach (StatLine line, mLinePorts) {
		//sort first by slope, then by current portId
		QMap<QPair<qreal, qreal>, EdgeElement*> sortedEdges;
		QLineF portLine = line;
		qreal dx = portLine.dx();
		qreal dy = portLine.dy();
		foreach (EdgeElement* edge, mEdgeList) {
			if (portId(edge->portIdOn(this)) == lpId) {
				QPointF conn = edge->connectionPoint(this);
				QPointF next = edge->nextFrom(this);
				qreal x1 = conn.x();
				qreal y1 = conn.y();
				qreal x2 = next.x();
				qreal y2 = next.y();
				qreal len = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
				qreal scalarProduct = ((x2 - x1) * dx + (y2 - y1) * dy) / len;
				sortedEdges.insertMulti(qMakePair(edge->portIdOn(this), scalarProduct), edge);
			}
		}

		//by now, edges of this port are sorted by their optimal slope.
		int N = sortedEdges.size();
		int i = 0;
		foreach (EdgeElement* edge, sortedEdges) {
			qreal newId = lpId + (1.0 + i++) / (N + 1);
			edge->moveConnection(this, newId);
		}

		lpId++; //next linear port.

	}
}

void NodeElement::arrangeLinks() {
	foreach (EdgeElement* edge, mEdgeList) {
		NodeElement* src = edge->src();
		NodeElement* dst = edge->dst();
		edge->reconnectToNearestPorts(this == src, this == dst, true);
	}

	arrangeLinearPorts();

	foreach (EdgeElement* edge, mEdgeList) {
		NodeElement* src = edge->src();
		NodeElement* dst = edge->dst();
		NodeElement* other = edge->otherSide(this);
		edge->reconnectToNearestPorts(other == src, other == dst, true);
	}

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

	if (QPolygon(mContents.toAlignedRect()) != mGraphicalAssistApi->configuration(id())) { // check if it's been changed
		mGraphicalAssistApi->setConfiguration(id(), QPolygon(contents.toAlignedRect()));
	}
}

void NodeElement::moveChildren(qreal dx, qreal dy)
{
	foreach (QGraphicsItem* childItem, childItems()) {
		NodeElement* curItem = dynamic_cast<NodeElement*>(childItem);
		if (curItem && !curItem->isPort()) {
			curItem->moveBy(dx, dy);
			///returns object to the parent area
			if (curItem->pos().x() < mElementImpl->sizeOfForestalling()) {
				curItem->setPos(mElementImpl->sizeOfForestalling(), curItem->pos().y());
			}
			if (curItem->pos().y() < mElementImpl->sizeOfForestalling()) {
				curItem->setPos(curItem->pos().x(), mElementImpl->sizeOfForestalling());
			}
		}
	}
}

void NodeElement::moveChildren(QPointF const &moving)
{
	moveChildren(moving.x(), moving.y());
}

void NodeElement::resize(QRectF newContents)
{
	newContents.moveTo(0, 0);
	if (mElementImpl->isSortingContainer()) {
		sortChildren();
	}

	if (mElementImpl->minimizesToChildren()) {
		newContents = QRectF(0, 0, 0, 0);
	}

	//childrenMoving - negative shift of children from the point (SIZE_OF_FORESTALLING, SIZE_OF_FORESTALLING)
	//whatever it means :)
	QPointF childrenMoving = QPointF(0, 0);
	foreach (QGraphicsItem *childItem, childItems()) {
		NodeElement* curItem = dynamic_cast<NodeElement*>(childItem);
		if (curItem && curItem->isPort() && newContents != mContents) {
			curItem->resizeChild(newContents, mContents);
		}

		if (!curItem || curItem->isPort()) {
			continue;
		}

		QPointF curItemPos = childItem->pos();

		if (curItemPos.x() < childrenMoving.x() + mElementImpl->sizeOfForestalling()) {
			childrenMoving.setX(curItemPos.x() - mElementImpl->sizeOfForestalling());
		}

		if (curItemPos.y() < childrenMoving.y() + mElementImpl->sizeOfForestalling()) {
			childrenMoving.setY(curItemPos.y() - mElementImpl->sizeOfForestalling());
		}
	}

//	setPos(pos() + childrenMoving);
	moveChildren(-childrenMoving);

	foreach (QGraphicsItem* childItem, childItems()) {
		QRectF curChildItemBoundingRect;
		if(childItem == mPlaceholder) {
			curChildItemBoundingRect = childItem->boundingRect();
			curChildItemBoundingRect.setLeft(newContents.left() + mElementImpl->sizeOfForestalling());
			curChildItemBoundingRect.setRight(newContents.right() - mElementImpl->sizeOfForestalling());
		} else {
			NodeElement* curItem = dynamic_cast<NodeElement*>(childItem);
			if (!curItem || curItem->isPort()) {
				continue;
			}
			curChildItemBoundingRect = curItem->mContents;
		}

		// it seems to be more appropriate to use childItem->pos() but it causes
		// bad behaviour when dropping one element to another
		curChildItemBoundingRect.translate(childItem->scenePos() - scenePos());

		if (curChildItemBoundingRect.left() < newContents.left() + mElementImpl->sizeOfForestalling()) {
			newContents.setLeft(curChildItemBoundingRect.left() - mElementImpl->sizeOfForestalling());
		}

		if (curChildItemBoundingRect.right() > newContents.right() - mElementImpl->sizeOfForestalling()) {
			newContents.setRight(curChildItemBoundingRect.right() + mElementImpl->sizeOfForestalling());
		}

		if (curChildItemBoundingRect.top() < newContents.top() + mElementImpl->sizeOfForestalling()) {
			newContents.setTop(curChildItemBoundingRect.top() - mElementImpl->sizeOfForestalling());
		}

		if (curChildItemBoundingRect.bottom() > newContents.bottom() - mElementImpl->sizeOfForestalling()) {
			newContents.setBottom(curChildItemBoundingRect.bottom() + mElementImpl->sizeOfForestalling());
		}
	}

	if ((newContents.width() < objectMinSize) || (newContents.height() < objectMinSize)) {
		newContents = mFoldedContents;
	}

	newContents.moveTo(pos());
	setGeometry(newContents);

	NodeElement *parItem = dynamic_cast<NodeElement*>(parentItem());
	if (parItem) {
		parItem->resize(parItem->mContents); // recursive expansion of parents
	}
}

QList<ContextMenuAction*> NodeElement::contextMenuActions()
{
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

	if (isSelected()) {
		if (QRectF(mContents.topLeft(), QSizeF(4, 4)).contains(event->pos()) && mElementImpl->isResizeable()) {
			mDragState = TopLeft;
		} else if (QRectF(mContents.topRight(), QSizeF(-4, 4)).contains(event->pos()) && mElementImpl->isResizeable()) {
			mDragState = TopRight;
		} else if (QRectF(mContents.bottomRight(), QSizeF(-12, -12)).contains(event->pos()) && mElementImpl->isResizeable()) {
			mDragState = BottomRight;
		} else if (QRectF(mContents.bottomLeft(), QSizeF(4, -4)).contains(event->pos()) && mElementImpl->isResizeable()) {
			mDragState = BottomLeft;
		} else if (QRectF(mContents.topLeft(), QSizeF(20, 20)).contains(event->pos()) && mElementImpl->isContainer()) {
			changeFoldState();
		} else {
			Element::mousePressEvent(event);
		}
	} else {
		Element::mousePressEvent(event);
	}

	mLeftPressed = true;
	setZValue(1);
}

void NodeElement::alignToGrid()
{
	mGrid->alignToGrid();
}

void NodeElement::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::RightButton) {
		event->accept();
		return;
	}

	scene()->invalidate();
	if (mDragState == None) {

		if (!isPort() && (flags() & ItemIsMovable)) {
			// in case of unresizable item use switch
			// Determing parent using corner position, not mouse coordinates
			QPointF newParentInnerPoint = event->scenePos();
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
				newParentInnerPoint = scenePos() + QPointF(0, mContents.height()/2);
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
				mHighlightedNode = newParent;
				mHighlightedNode->drawPlaceholder(EditorViewScene::getPlaceholder(), event->scenePos());
			} else if (mHighlightedNode != NULL) {
				mHighlightedNode->erasePlaceholder(true);
				mHighlightedNode = NULL;
			}
		}

		Element::mouseMoveEvent(event);
		mGrid->mouseMoveEvent(event);

	} else if (mElementImpl->isResizeable()) {
		setVisibleEmbeddedLinkers(false);

		QRectF newContents = mContents;
		QPointF parentPos = QPointF(0, 0);
		QGraphicsItem *parItem = parentItem();
		if (parItem) {
			parentPos = parItem->scenePos();
		}

		qreal const newX = mGrid->makeGridAlignment(event->pos().x());
		qreal const newY = mGrid->makeGridAlignment(event->pos().y());
		switch (mDragState) {
		case TopLeft: {
			newContents.setTopLeft(QPoint(newX, newY));
			setPos(event->scenePos() - parentPos);
			break;
		}
		case Top: {
			newContents.setTop(newY);
			setPos(pos().x(), event->scenePos().y() - parentPos.y());
			break;
		}
		case TopRight: {
			newContents.setTopRight(QPoint(newX, newY));
			setPos(pos().x(), event->scenePos().y() - parentPos.y());
			break;
		}
		case Left: {
			newContents.setLeft(newX);
			setPos(event->scenePos().x() - parentPos.x(), pos().y());
			break;
		}
		case Right: {
			newContents.setRight(newX);
			break;
		}
		case BottomLeft: {
			newContents.setBottomLeft(QPoint(newX, newY));
			setPos(event->scenePos().x() - parentPos.x(), pos().y());
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
			qreal size = std::max(newContents.width(), newContents.height());
			newContents.setWidth(size);
			newContents.setHeight(size);
		}

		resize(newContents);
	}

	if (isPort()) {
		mUmlPortHandler->handleMoveEvent(
				  mLeftPressed
				, mPos
				, event->scenePos()
				, mParentNodeElement
			);
	}

	arrangeLinks();
}

void NodeElement::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::RightButton) {
		event->accept();
		return;
	}
	delUnusedLines();
	mContents = mContents.normalized();
	storeGeometry();

	setVisibleEmbeddedLinkers(true);

	if (mDragState == None) {
		Element::mouseReleaseEvent(event);
	}

	// we should use mHighlightedNode to determine if there is a highlighted node
	// insert current element into them and set mHighlightedNode to NULL
	// but because of mouseRelease twice triggering we can't do it
	// This may cause more bugs
	if (!isPort() && (flags() & ItemIsMovable)) {
		EditorViewScene *evScene = dynamic_cast<EditorViewScene *>(scene());
		if (mHighlightedNode != NULL) {
			NodeElement *newParent = mHighlightedNode;
			Element *insertBefore = mHighlightedNode->getPlaceholderNextElement();
			mHighlightedNode->erasePlaceholder(false);
			// commented because of bug with double event sending (see #204)
	//		mHighlightedNode = NULL;

			mGraphicalAssistApi->changeParent(id(), newParent->id(),
				mapToItem(evScene->getElem(newParent->id()), mapFromScene(scenePos())));

			if (insertBefore != NULL) {
				mGraphicalAssistApi->stackBefore(id(), insertBefore->id());
			}

			newParent->resize(newParent->mContents);

			while (newParent != NULL) {
				newParent->mContents = newParent->mContents.normalized();
				newParent->storeGeometry();
				newParent = dynamic_cast<NodeElement*>(newParent->parentItem());
			}
		} else {
			mGraphicalAssistApi->changeParent(id(), evScene->rootItemId(), scenePos());
		}
	}

	mDragState = None;
	setZValue(0);
}

void NodeElement::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	Q_UNUSED(event)
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

	EditorInterface const * const editorInterface = mGraphicalAssistApi->editorManager().editorInterface(id().editor());
	foreach (QString elementName, editorInterface->elements(id().diagram())) {
		int ne = editorInterface->isNodeOrEdge(elementName);
		if (ne == -1) {
			QList<StringPossibleEdge> list = editorInterface->getPossibleEdges(elementName);
			foreach(StringPossibleEdge pEdge, list) {
				if (editorInterface->isParentOf(id().diagram(), pEdge.first.first, id().diagram(), id().element())
						|| (editorInterface->isParentOf(id().diagram(), pEdge.first.second, id().diagram(), id().element()) && !pEdge.second.first))
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
	foreach (EmbeddedLinker* embeddedLinker, mEmbeddedLinkers) {
		embeddedLinker->initTitle();
	}
}

void NodeElement::setVisibleEmbeddedLinkers(bool const show)
{
	if (show) {
		int index = 0;
		int maxIndex = mEmbeddedLinkers.size();
		foreach (EmbeddedLinker* embeddedLinker, mEmbeddedLinkers) {
			embeddedLinker->takePosition(index,maxIndex);
			embeddedLinker->show();
			index++;
		}
	} else {
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
	return mContents.adjusted(-kvadratik, -kvadratik, kvadratik, kvadratik);
}

void NodeElement::updateData()
{
	Element::updateData();
	if (mMoving == 0) {
		QPointF newpos = mGraphicalAssistApi->position(id());
		QPolygon newpoly = mGraphicalAssistApi->configuration(id());
		QRectF newRect; // Use default ((0,0)-(0,0))
		// QPolygon::boundingRect is buggy :-(
		if (!newpoly.isEmpty()) {
			int minx = newpoly[0].x();
			int miny = newpoly[0].y();
			int maxx = newpoly[0].x();
			int maxy = newpoly[0].y();;
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

int NodeElement::portId(qreal id)
{
	int iid = qRound(id);
	if (id < 1.0 * iid) {
		return iid - 1;
	} else {
		return iid;
	}
}

const QPointF NodeElement::getPortPos(qreal id) const
{
	int iid = portId(id);

	if (id < 0.0) {
		return QPointF(0, 0);
	}

	if (id < mPointPorts.size()) {
		return newTransform(mPointPorts[iid]);
	}

	if (id < mPointPorts.size() + mLinePorts.size()) {
		return newTransform(mLinePorts.at(iid - mPointPorts.size())).pointAt(id - 1.0 * iid);
	} else {
		return QPointF(0, 0);
	}
}

const QPointF NodeElement::getNearestPort(QPointF const &location) const
{
	QPointF min;

	if (mPointPorts.size() > 0) {
		QPointF const pointPort = newTransform(mPointPorts[0]);
		min.setX(pointPort.x() + boundingRect().left());
		min.setY(pointPort.y() + boundingRect().top());

		foreach (StatPoint port, mPointPorts) {
			QPointF const pointPort = newTransform(port);
			port.point.setX(pointPort.x() + boundingRect().left());
			port.point.setY(pointPort.y() + boundingRect().top());
			if (QLineF(port.point, location).length() < QLineF(min, location).length()) {
				min = port.point;
			}
		}
		return min;
	}

	if (mLinePorts.size() > 0) {
		int num = 0;
		foreach (StatLine line, mLinePorts) {
			qreal k = qMin(qMax(0., getNearestPointOfLinePort(num, location)), 0.9999);
			QLineF const &sceneLine = newTransform(line);
			QPointF const &port = sceneLine.pointAt(k);

			if (QLineF(port, location).length() < QLineF(min, location).length() || num == 0) {
				min = port;
			}
			num++;
		}

		return min;
	}

	return location;
}

QLineF NodeElement::newTransform(StatLine const &port) const
{
	float x1 = 0.0;
	float x2 = 0.0;
	float y1 = 0.0;
	float y2 = 0.0;

	if (port.prop_x1) {
		x1 = port.line.x1() * port.initWidth;
	} else {
		x1 = port.line.x1() * contentsRect().width();
	}

	if (port.prop_y1) {
		y1 = port.line.y1() * port.initHeight;
	} else {
		y1 = port.line.y1() * contentsRect().height();
	}

	if (port.prop_x2) {
		x2 = port.line.x2() * port.initWidth;
	} else {
		x2 = port.line.x2() * contentsRect().width();
	}

	if (port.prop_y2) {
		y2 = port.line.y2() * port.initHeight;
	} else {
		y2 = port.line.y2() * contentsRect().height();
	}

	return QLineF(x1, y1, x2, y2);
}

QPointF NodeElement::newTransform(StatPoint const &port) const
{
	qreal x = 0;
	qreal y = 0;

	if (port.prop_x) {
		x = port.point.x() * port.initWidth;
	} else {
		x = port.point.x() * contentsRect().width();
	}

	if (port.prop_y) {
		y = port.point.y() * port.initHeight;
	} else {
		y = port.point.y() * contentsRect().height();
	}

	return QPointF(x, y);
}

qreal NodeElement::minDistanceFromLinePort(int linePortNumber, QPointF const &location) const
{
	QLineF linePort = newTransform(mLinePorts[linePortNumber]);
	qreal a = linePort.length();
	qreal b = QLineF(linePort.p1(), location).length();
	qreal c = QLineF(linePort.p2(), location).length();

	qreal nearestPointOfLinePort = getNearestPointOfLinePort(linePortNumber, location);
	if ((nearestPointOfLinePort < 0) || (nearestPointOfLinePort > 0.9999)) {
		return qMin(b, c);
	} else {
		qreal p = (a + b + c) / 2;
		qreal triangleSquare = sqrt(p * (p - a) * (p - b) * (p - c));
		qreal minDistance = 2 * triangleSquare / a;
		return minDistance;
	}
}

qreal NodeElement::distanceFromPointPort(int pointPortNumber, QPointF const &location) const
{
	return QLineF(newTransform(mPointPorts[pointPortNumber]), location).length();
}

qreal NodeElement::getNearestPointOfLinePort(int linePortNumber, QPointF const &location) const
{
	qreal nearestPointOfLinePort = 0;
	QLineF nearestLinePort = newTransform(mLinePorts[linePortNumber]);
	qreal y1 = nearestLinePort.y1(),
			y2 = nearestLinePort.y2(),
			x1 = nearestLinePort.x1(),
			x2 = nearestLinePort.x2();

	if (x1 == x2) {
		nearestPointOfLinePort = (location.y() - y1) / (y2 - y1);
	} else if (y1 == y2) {
		nearestPointOfLinePort = (location.x() - x1) / (x2 - x1);
	} else {
		qreal k = (y2 - y1) / (x2 - x1),
				b2 = location.y() + 1 / k * location.x(),
				b = y1 - k * x1,
				x3 = k / (1 + k * k) * (b2 - b);
		nearestPointOfLinePort = (x3 - x1) / (x2 - x1);
	}
	return nearestPointOfLinePort;
}

qreal NodeElement::getPortId(QPointF const&location) const
{
	for (int i = 0; i < mPointPorts.size(); ++i) {
		if (QRectF(newTransform(mPointPorts[i]) - QPointF(kvadratik, kvadratik),
				   QSizeF(kvadratik * 2, kvadratik * 2)).contains(location))
		{
			return 1.0 * i;
		}
	}

	for (int i = 0; i < mLinePorts.size(); i++) {
		QPainterPathStroker ps;
		ps.setWidth(kvadratik - 5);

		QPainterPath path;
		path.moveTo(newTransform(mLinePorts[i]).p1());
		path.lineTo(newTransform(mLinePorts[i]).p2());

		path = ps.createStroke(path);
		if (path.contains(location)) {
			return (1.0 * (i + mPointPorts.size()) + qMin(0.9999,
				QLineF(QLineF(newTransform(mLinePorts[i])).p1(), location).length()
				 / newTransform(mLinePorts[i]).length()));
		}
	}

	qreal minDistance = 0;
	int numMinDistance = -1;
	if (mPointPorts.size() != 0) {
		numMinDistance = 0;
		minDistance = distanceFromPointPort(0, location);
		for (int i = 1; i < mPointPorts.size(); i++) {
			qreal currentDistance = distanceFromPointPort(i, location);
			if (currentDistance < minDistance) {
				numMinDistance = i;
				minDistance = currentDistance;
			}
		}
	}

	if (mLinePorts.size() != 0) {
		bool linePort = false;
		if (numMinDistance == -1) {
			numMinDistance = 0;
			minDistance = minDistanceFromLinePort(0, location);
			linePort = true;
		}
		for (int i = 0; i < mLinePorts.size(); i++) {
			qreal currentDistance = minDistanceFromLinePort(i, location);
			if (currentDistance < minDistance) {
				numMinDistance = i;
				minDistance = currentDistance;
				linePort = true;
			}
		}
		if (linePort) {
			qreal nearestPointOfLinePort = getNearestPointOfLinePort(numMinDistance, location);
			if (nearestPointOfLinePort < 0) {
				nearestPointOfLinePort = 0;
			} else if (nearestPointOfLinePort > 0.9999) {
				nearestPointOfLinePort = 0.9999;
			}
			return 1.0 * (numMinDistance + nearestPointOfLinePort + mPointPorts.size());
		} else {
			return 1.0 * numMinDistance;
		}
	} else if (numMinDistance >= 0) {
		return 1.0 * numMinDistance;
	}

	return -1.0;
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

void NodeElement::paint(QPainter *painter, QStyleOptionGraphicsItem const *style, QWidget *widget)
{
	mElementImpl->paint(painter, mContents);
	if (mElementImpl->hasPorts()) {
		paint(painter, style, widget, mPortRenderer);
	} else {
		paint(painter, style, widget, 0);
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

void NodeElement::paint(QPainter *painter, QStyleOptionGraphicsItem const *option,
						QWidget*, SdfRenderer* portRenderer)
{
	if (option->levelOfDetail >= 0.5) {
		if (option->state & QStyle::State_Selected) {
			painter->save();

			QBrush b;
			b.setStyle(Qt::SolidPattern);

			if (mIsFolded) {
				b.setColor(Qt::red);
				painter->setPen(Qt::red);
			}
			else {
				b.setColor(Qt::green);
				painter->setPen(Qt::green);
			}
			painter->setBrush(b);

			if (mElementImpl->isContainer()) {
				painter->drawRect(QRectF(mContents.topLeft(), QSizeF(20, 20)));
			}

			b.setColor(Qt::blue);
			painter->setBrush(b);
			painter->setPen(Qt::blue);

			painter->drawRect(QRectF(mContents.topLeft(), QSizeF(4, 4)));
			painter->drawRect(QRectF(mContents.topRight(), QSizeF(-4, 4)));
			painter->drawRect(QRectF(mContents.bottomLeft(), QSizeF(4, -4)));

			if (mElementImpl->isResizeable()) {
				painter->translate(mContents.bottomRight());
				painter->drawLine(QLineF(-4, 0, 0, -4));
				painter->drawLine(QLineF(-8, 0, 0, -8));
				painter->drawLine(QLineF(-12, 0, 0, -12));
			} else {
				painter->drawRect(QRectF(mContents.bottomRight(), QSizeF(4, 4)));
			}

			painter->restore();
		}
		if (((option->state & QStyle::State_MouseOver) || mPortsVisible) && portRenderer) {
			painter->save();
			painter->setOpacity(0.7);
			portRenderer->render(painter, mContents);
			painter->restore();
		}
	}
}

void NodeElement::addEdge(EdgeElement *edge)
{
	mEdgeList << edge;
}

void NodeElement::delEdge(EdgeElement *edge)
{
	mEdgeList.removeAt(mEdgeList.indexOf(edge));
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
		parent->resize(parent->mContents);
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

void NodeElement::sortChildren()
{
	qreal curChildY = mElementImpl->sizeOfForestalling() + titlePadding;
	qreal maxChildrenWidth = 0;

	foreach (QGraphicsItem* childItem, childItems()) {
		NodeElement* curItem = dynamic_cast<NodeElement*>(childItem);
		if (curItem) {
			if (curItem->mContents.width() > maxChildrenWidth) {
				maxChildrenWidth = curItem->mContents.width();
			}
		}
	}

	if (maxChildrenWidth == 0) {
		maxChildrenWidth = childrenBoundingRect().width();
	}

	foreach (QGraphicsItem* childItem, childItems()) {
		NodeElement* curItem = dynamic_cast<NodeElement*>(childItem);
		if(mPlaceholder != NULL && childItem == mPlaceholder){
			QRectF rect(mElementImpl->sizeOfForestalling(), curChildY,
				maxChildrenWidth, mPlaceholder->rect().height());
			mPlaceholder->setRect(rect);
			curChildY += mPlaceholder->rect().height() + childSpacing;
		}
		if (curItem) {
			QRectF rect(mElementImpl->sizeOfForestalling(), curChildY, 0, curItem->mContents.height());
			if (mElementImpl->maximizesChildren()) {
				rect.setWidth(maxChildrenWidth);
			} else {
				rect.setWidth(curItem->mContents.width());
			}
			curItem->setGeometry(rect);
			curItem->storeGeometry();
			curChildY += curItem->mContents.height() + mElementImpl->sizeOfChildrenForestalling() + childSpacing;
		}
	}
}

void NodeElement::drawPlaceholder(QGraphicsRectItem *placeholder, QPointF pos)
{
	// for non-sorting containers no need for drawing placeholder so just make them marked
	if (!mElementImpl->isSortingContainer()) {
		setOpacity(.2);
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
	if(nextItem != NULL){
		mPlaceholder->stackBefore(nextItem);
	}

	resize(QRectF());
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
	if(mPlaceholder != NULL){
		delete mPlaceholder;
		mPlaceholder = NULL;
	}
	if(redraw){
		resize(QRectF());
	}
}

bool NodeElement::isPort()
{
	return mElementImpl->isPort();
}

bool NodeElement::canHavePorts()
{
	return mElementImpl->hasPin();
}

bool NodeElement::checkLowerBorder(QPointF& point, double x, double y) const
{
	double currentX = point.x();
	double currentY = point.y();
	QRectF rc = boundingRect();
	return (currentX >= rc.x() + x) && (currentX <= rc.x() + rc.width() - x) && (currentY >= rc.y() + rc.height() - y)
			&& (currentY <= rc.y() + rc.height() + y);
}

bool NodeElement::checkUpperBorder(QPointF& point, double x, double y) const
{
	double currentX = point.x();
	double currentY = point.y();
	QRectF rc = boundingRect();
	return (currentX >= rc.x() + x) && (currentX <= rc.x() + rc.width() - x) && (currentY >= rc.y() - y)
			&& (currentY <= rc.y() + y);
}

bool NodeElement::checkLeftBorder(QPointF& point, double x, double y) const
{
	double currentX = point.x();
	double currentY = point.y();
	QRectF rc = boundingRect();
	return (currentX >= rc.x() - x) && (currentX <= rc.x() + x) && (currentY >= rc.y() + y)
			&& (currentY <= rc.y() + rc.height() - y);
}

bool NodeElement::checkRightBorder(QPointF& point, double x, double y) const
{
	double currentX = point.x();
	double currentY = point.y();
	QRectF rc = boundingRect();
	return (currentX >= rc.x() + rc.width() - x) && (currentX <= rc.x() + rc.width() + x) && (currentY >= rc.y() + y)
			&& (currentY <= rc.y() + rc.height() - y);
}

bool NodeElement::checkNoBorderX(QPointF& point, double x, double y) const
{
	Q_UNUSED(x);
	double currentY = point.ry();
	QRectF rc = boundingRect();
	return (currentY >= rc.y() + y) && (currentY <= rc.y() + rc.height() - y);
}

bool NodeElement::checkNoBorderY(QPointF& point, double x, double y) const
{
	Q_UNUSED(y);
	double currentX = point.rx();
	QRectF rc = boundingRect();
	return (currentX >= rc.x() + x) && (currentX <= rc.x() + rc.width() - x);
}

void NodeElement::resizeChild(QRectF newContents, QRectF oldContents)
{
	if (!mParentNodeElement) {
		QGraphicsItem* item = parentItem();
		mParentNodeElement = dynamic_cast<NodeElement*>(item);
	}

	if (mPos == QPointF(0,0)) {
		mPos = pos();
	}
	QList<double> list = mParentNodeElement->borderValues();
	double xHor = list[0];
	double yHor = list[1];
	double xVert = list[2];
	double yVert = list[3];
	QPointF posi = pos();
	if (mParentNodeElement->checkLowerBorder(posi, xHor, yHor+5)) {
		double x = mPos.x() - oldContents.x();
		double a = oldContents.x() + oldContents.width();
		double b = newContents.x() + newContents.width();
		double dy = newContents.height() - oldContents.height();
		mPos = QPointF(newContents.x() + x*b/a, mPos.y()+dy);
		setPos(mPos);
	}

	if (mParentNodeElement->checkUpperBorder(posi, xHor, yHor)) {
		double x = mPos.x() - oldContents.x();
		double a = oldContents.x() + oldContents.width();
		double b = newContents.x() + newContents.width();
		double dy = 0;
		mPos = QPointF(newContents.x() + x*b/a, mPos.y()+dy);
		setPos(mPos);
	}

	if (mParentNodeElement->checkRightBorder(posi, xVert+5, yVert)) {
		double y = mPos.y() - oldContents.y();
		double a = oldContents.y() + oldContents.height();
		double b = newContents.y() + newContents.height();
		double dx = newContents.width() - oldContents.width();
		mPos = QPointF(mPos.x()+dx, newContents.y() + y*b/a);
		setPos(mPos);
	}
	if (mParentNodeElement->checkLeftBorder(posi, xVert, yVert))
	{
		double y = mPos.y() - oldContents.y();
		double a = oldContents.y() + oldContents.height();
		double b = newContents.y() + newContents.height();
		double dx = 0;
		mPos = QPointF(mPos.x()+dx, newContents.y() + y*b/a);
		setPos(mPos);
	}
	storeGeometry();
	return;
}

void NodeElement::updateByChild(NodeElement* item, bool isItemAddedOrDeleted)
{
	if (mIsFolded && isItemAddedOrDeleted && (item != 0)) {
		changeFoldState();
	}

	/*
	QRectF newContents = mContents;
//	newContents.moveTo(pos());
	QRectF itemContents = item->mContents;
	itemContents.moveTo(item->pos() - pos());

	newContents = newContents.united(itemContents);
	resize(mContents.unite(newContents));
 */
	resize(mContents);
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

QList<double> NodeElement::borderValues()
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

void NodeElement::connectTemporaryRemovedLinksToPort(IdList const &temporaryRemovedLinks, QString const &direction)
{
	foreach (Id edgeId, temporaryRemovedLinks) {
		EdgeElement *edge = dynamic_cast<EdgeElement *>(static_cast<EditorViewScene *>(scene())->getElem(edgeId));
		if (edge != NULL) {
			if (direction == "from") {
				QPointF startPos = edge->mapFromItem(this, this->getNearestPort(edge->line().first()));
				edge->placeStartTo(startPos);
			}
			else {
				QPointF endPos = edge->mapFromItem(this, this->getNearestPort(edge->line().last()));
				edge->placeEndTo(endPos);
			}
			edge->connectToPort();
		}
	}
}

void NodeElement::checkConnectionsToPort()
{
	connectTemporaryRemovedLinksToPort(mGraphicalAssistApi->temporaryRemovedLinksFrom(id()), "from");
	connectTemporaryRemovedLinksToPort(mGraphicalAssistApi->temporaryRemovedLinksTo(id()), "to");
	connectTemporaryRemovedLinksToPort(mGraphicalAssistApi->temporaryRemovedLinksNone(id()), QString());
	mGraphicalAssistApi->removeTemporaryRemovedLinks(id());

	// i have no idea what this method does, but it is called when the element
	// is dropped on scene. so i'll just leave this code here for now.
	QList<QGraphicsItem *>  items = scene()->items(scenePos());
	EdgeElement *edge = NULL;
	foreach (QGraphicsItem *item, items) {
		edge = dynamic_cast<EdgeElement *>(item);
		if (edge) {
			edge->connectToPort();
			return;
		}
	}
}

void NodeElement::singleSelectionState(bool const singleSelected)
{
	initEmbeddedLinkers();
	setVisibleEmbeddedLinkers(singleSelected);
	Element::singleSelectionState(singleSelected);
}

void NodeElement::selectionState(bool const selected)
{
	Element::selectionState(selected);
}

void NodeElement::highlightEdges()
{
	foreach (EdgeElement *edge, mEdgeList)
		edge->highlight();
}
