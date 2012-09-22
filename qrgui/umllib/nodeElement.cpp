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
#include "private/resizeHandler.h"
#include "private/copyHandler.h"

using namespace qReal;

NodeElement::NodeElement(ElementImpl* impl)
	: Element(impl)
	, mSwitchGridAction(tr("Switch on grid"), this)
	, mPortsVisible(false)
	, mDragState(None)
	, mIsFolded(false)
	, mLeftPressed(false)
	, mParentNodeElement(NULL)
	, mPos(QPointF(0,0))
	, mSelectionNeeded(false)
	, mConnectionInProgress(false)
	, mPlaceholder(NULL)
	, mHighlightedNode(NULL)
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
	mElementImpl->init(mContents, pointPorts, linePorts, factory, titles, mRenderer, mPortRenderer);
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

QMap<QString, QVariant> NodeElement::properties()
{
	return mGraphicalAssistApi->properties(id());
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
		//sort first by slope, then by current portNumber
		QMap<QPair<qreal, qreal>, EdgeElement*> sortedEdges;
		QLineF portLine = line;
		qreal dx = portLine.dx();
		qreal dy = portLine.dy();
		foreach (EdgeElement* edge, mEdgeList) {
			if (portNumber(edge->portIdOn(this)) == lpId) {
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
	mPortHandler->arrangeLinearPorts();
}

void NodeElement::arrangeLinks() {
	//Episode I: Home Jumps
	//qDebug() << "I";
	foreach (EdgeElement* edge, mEdgeList) {
		NodeElement* src = edge->src();
		NodeElement* dst = edge->dst();
		edge->reconnectToNearestPorts(this == src, this == dst, true);
	}

	//Episode II: Home Ports Arranging
	//qDebug() << "II";
	arrangeLinearPorts();

	//Episode III: Remote Jumps
	//qDebug() << "III";
	foreach (EdgeElement* edge, mEdgeList) {
		NodeElement* src = edge->src();
		NodeElement* dst = edge->dst();
		NodeElement* other = edge->otherSide(this);
		edge->reconnectToNearestPorts(other == src, other == dst, true);
	}

	//Episode IV: Remote Arrangigng
	//qDebug() << "IV";
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

	mDragPosition = event->scenePos() - scenePos();

	mLeftPressed = true;
	setZValue(1);

	if (!isPort() && (flags() & ItemIsMovable)) {
		recalculateHighlightedNode(event->scenePos());
	}
}

void NodeElement::alignToGrid()
{
	mGrid->alignToGrid();
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

	scene()->invalidate();
	if (mDragState == None) {
		if (!isPort() && (flags() & ItemIsMovable)) {
			recalculateHighlightedNode(event->scenePos());
		}

		newPos += (event->scenePos() - scenePos()) - mDragPosition;
		mGrid->mouseMoveEvent(event);
		alignToGrid();

	} else if (mElementImpl->isResizeable()) {
		setVisibleEmbeddedLinkers(false);

		QPointF parentPos = QPointF(0, 0);
		QGraphicsItem *parItem = parentItem();
		if (parItem) {
			parentPos = parItem->scenePos();
		}

		qreal const newX = event->pos().x();
		qreal const newY = event->pos().y();

		switch (mDragState) {
		case TopLeft: {
			newContents.setTopLeft(QPoint(newX, newY));
			newPos = event->scenePos() - parentPos;
			break;
		}
		case Top: {
			newContents.setTop(newY);
			newPos = QPoint(pos().x(), event->scenePos().y() - parentPos.y());
			break;
		}
		case TopRight: {
			newContents.setTopRight(QPoint(newX, newY));
			newPos = QPoint(pos().x(), event->scenePos().y() - parentPos.y());
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
			newContents.setBottomLeft(QPoint(newX, newY));
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

	resize(newContents, newPos);

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

	if (SettingsManager::value("ActivateGrid").toBool()) {
		if (isParentSortingContainer()) {
			alignToGrid();
		}
	}
	storeGeometry();

	setVisibleEmbeddedLinkers(true);

	if (mDragState == None) {
		Element::mouseReleaseEvent(event);
	}

	EditorViewScene *evScene = dynamic_cast<EditorViewScene *>(scene());
	evScene->insertNodeIntoEdge(id(), Id::rootId(), false, event->scenePos());

	// we should use mHighlightedNode to determine if there is a highlighted node
	// insert current element into them and set mHighlightedNode to NULL
	// but because of mouseRelease twice triggering we can't do it
	// This may cause more bugs
	if (!isPort() && (flags() & ItemIsMovable)) {
		if (mHighlightedNode != NULL) {
			NodeElement *newParent = mHighlightedNode;
			Element *insertBefore = mHighlightedNode->getPlaceholderNextElement();
			mHighlightedNode->erasePlaceholder(false);
			// commented because of bug with double event sending (see #204)
	//		mHighlightedNode = NULL;

			QPointF newPos = mapToItem(newParent, mapFromScene(scenePos()));
			mGraphicalAssistApi->changeParent(id(), newParent->id(), newPos);
			setPos(newPos);

			if (insertBefore != NULL) {
				mGraphicalAssistApi->stackBefore(id(), insertBefore->id());
			}

			newParent->resize();

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
	Q_UNUSED(event);

	if (mElementImpl->isContainer()) {
		changeFoldState();
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
	if (!mMoving) {
		storeGeometry();
		QPointF newpos = mGraphicalAssistApi->position(id());
		QPolygon newpoly = mGraphicalAssistApi->configuration(id()); // why is it empty?
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
	if(mPlaceholder != NULL){
		delete mPlaceholder;
		mPlaceholder = NULL;
	}
	if(redraw){
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

/*
void NodeElement::resizeChild(QRectF const &newContents, QRectF const &oldContents)
{
	if (!mParentNodeElement) {
		QGraphicsItem* item = parentItem();
		mParentNodeElement = dynamic_cast<NodeElement*>(item);
	}

	if (mPos == QPointF(0,0)) {
		mPos = pos();
	}
	QList<double> list = mParentNodeElement->borderValues();
	double const xHor = list[0];
	double const yHor = list[1];
	double const xVert = list[2];
	double const yVert = list[3];
	QPointF const posi = pos();

	double const x = mPos.x() - oldContents.x();
	double const y = mPos.y() - oldContents.y();

	if (mParentNodeElement->checkLowerBorder(posi, xHor, yHor+5)) {
		double const a = oldContents.x() + oldContents.width();
		double const b = newContents.x() + newContents.width();
		double const dy = newContents.height() - oldContents.height();
		mPos = QPointF(newContents.x() + x*b/a, mPos.y()+dy);
	}

	if (mParentNodeElement->checkUpperBorder(posi, xHor, yHor)) {
		double const a = oldContents.x() + oldContents.width();
		double const b = newContents.x() + newContents.width();
		double const dy = 0;
		mPos = QPointF(newContents.x() + x*b/a, mPos.y()+dy);
	}

	if (mParentNodeElement->checkRightBorder(posi, xVert+5, yVert)) {
		double const a = oldContents.y() + oldContents.height();
		double const b = newContents.y() + newContents.height();
		double const dx = newContents.width() - oldContents.width();
		mPos = QPointF(mPos.x()+dx, newContents.y() + y*b/a);
	}
	if (mParentNodeElement->checkLeftBorder(posi, xVert, yVert))
	{
		double const a = oldContents.y() + oldContents.height();
		double const b = newContents.y() + newContents.height();
		double const dx = 0;
		mPos = QPointF(mPos.x()+dx, newContents.y() + y*b/a);
	}

	setPos(mPos);
	storeGeometry();
	return;
}
*/

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

void NodeElement::checkConnectionsToPort()
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
	mData.properties = properties();
	// new element should not have references to links connected to original source element
	mData.properties["links"] = IdListHelper::toVariant(IdList());
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

void NodeElement::resize(QRectF newContents)
{
	resize(newContents, pos());
}

void NodeElement::resize(QRectF newContents, QPointF newPos)
{
	ResizeHandler handler(this, mElementImpl);
	handler.resize(newContents, newPos);
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

void NodeElement::setAssistApi(qReal::models::GraphicalModelAssistApi *graphicalAssistApi, qReal::models::LogicalModelAssistApi *logicalAssistApi)
{
	Element::setAssistApi(graphicalAssistApi, logicalAssistApi);
	mPortHandler->setGraphicalAssistApi(graphicalAssistApi);
}

bool NodeElement::isParentSortingContainer() const {
	return (mParentNodeElement != NULL) && mParentNodeElement->mElementImpl->isSortingContainer();
}
