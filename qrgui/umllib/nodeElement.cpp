#include "nodeElement.h"
#include "../view/editorViewScene.h"
#include "../pluginInterface/editorInterface.h"

#include <QtGui/QStyle>
#include <QtGui/QStyleOptionGraphicsItem>
#include <QtGui/QMessageBox>
#include <QtGui/QTextCursor>
#include <QtGui/QToolTip>
#include <QtCore/QDebug>
#include <QtCore/QUuid>

#include <math.h>

using namespace qReal;

NodeElement::NodeElement(ElementImpl* impl)
	: mSwitchGridAction(tr("Switch on grid"), this),
	mPortsVisible(false), mDragState(None), mElementImpl(impl), mIsFolded(false),
	mLeftPressed(false), mParentNodeElement(NULL), mPos(QPointF(0,0)),
	mSelectionNeeded(false), mConnectionInProgress(false),
	mPlaceholder(NULL)
{
	setAcceptHoverEvents(true);
	setFlag(ItemClipsChildrenToShape, false);

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

void NodeElement::arrangeLinksRecursively(QSet<NodeElement*>& toArrange, QSet<NodeElement*>& arranged)
{
	toArrange.remove(this);

	foreach (EdgeElement* edge, mEdgeList) {
		NodeElement* src = edge->src();
		NodeElement* dst = edge->dst();
		edge->reconnectToNearestPorts(this == src || !arranged.contains(src), this == dst || !arranged.contains(dst));
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
				sortedEdges.insertMulti(qMakePair(scalarProduct, edge->portIdOn(this)), edge);
			}
		}

		//by now, edges of this port are sorted by their optimal slope.
		int N = sortedEdges.size();
		int i = 0;
		foreach (EdgeElement* edge, sortedEdges) {
			edge->moveConnection(this, lpId + (1.0 + i++) / (N + 1));
		}

		lpId++; //next linear port.

	}

	//recursive calls
	arranged.insert(this);
	while (!toArrange.isEmpty()) {
		NodeElement *next = *toArrange.begin();
		next->arrangeLinksRecursively(toArrange, arranged);
	}
}

void NodeElement::storeGeometry()
{
	QRectF contents = mContents; //для того, чтобы оставалась правильная конфигурация
	mGraphicalAssistApi->setPosition(id(), pos());
	mGraphicalAssistApi->setConfiguration(id(), QPolygon(contents.toAlignedRect()));
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
	foreach (QGraphicsItem* childItem, childItems()) {
		NodeElement* curItem = dynamic_cast<NodeElement*>(childItem);
		if (curItem && (curItem->isPort() && (newContents != mContents))) {
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
	setPos(pos() + childrenMoving);
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
		// bad behaviour when dropping element to another
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

	NodeElement* parItem = dynamic_cast<NodeElement*>(parentItem());
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

void NodeElement::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::RightButton) {
		event->accept();
		return;
	}

	scene()->invalidate();
	if (mDragState == None) {
		Element::mouseMoveEvent(event);
		mGrid->mouseMoveEvent();
	} else if (mElementImpl->isResizeable()) {
		setVisibleEmbeddedLinkers(false);

		QRectF newContents = mContents;
		QPointF parentPos = QPointF(0, 0);
		QGraphicsItem* parItem = parentItem();
		if (parItem) {
			parentPos = parItem->scenePos();
		}

		switch (mDragState) {
		case TopLeft:
			newContents.setTopLeft(event->pos());
			setPos(event->scenePos() - parentPos);
			break;
		case Top:
			newContents.setTop(event->pos().y());
			setPos(pos().x(), event->scenePos().y() - parentPos.y());
			break;
		case TopRight:
			newContents.setTopRight(event->pos());
			setPos(pos().x(), event->scenePos().y() - parentPos.y());
			break;
		case Left:
			newContents.setLeft(event->pos().x());
			setPos(event->scenePos().x() - parentPos.x(), pos().y());
			break;
		case Right:
			newContents.setRight(event->pos().x());
			break;
		case BottomLeft:
			newContents.setBottomLeft(event->pos());
			setPos(event->scenePos().x() - parentPos.x(), pos().y());
			break;
		case Bottom:
			newContents.setBottom(event->pos().y());
			break;
		case BottomRight:
			newContents.setBottomRight(event->pos());
			break;
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
			mLeftPressed,
			mPos,
			event->scenePos(),
			mParentNodeElement
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

	if (!isPort() && (flags() & ItemIsMovable)) {
		QPointF newParentInnerPoint = event->scenePos();
		//switch нужен для случая, когда мы не можем растягивать объект.
		//Его родитель должен определяться не по позиции мышки, а по позиции угла.
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

		// when we select multiple elements and move them, position of mouse release event could be
		// exactly over one of them. so to prevent handling this situation as putting all others in
		// container, we check if new parent is selected right now.

		// if we want to put multiple elements in a container, we should take scene()->items()
		// and remove elements that are currently selected from it.

		QList<QGraphicsItem *> parentsList;
		foreach (QGraphicsItem *item, scene()->items(newParentInnerPoint)) {
			NodeElement *e = dynamic_cast<NodeElement *>(item);
			if (e && (item != this)) {
				parentsList.append(e);
			}
		}

		EditorViewScene *evScene = dynamic_cast<EditorViewScene *>(scene());
		QList<QGraphicsItem *> selected = evScene->selectedItems();

		// but there could be a situation that we're trying to move element and it's container
		// together. it that case we should not change parent of this element

		if (!parentItem() || !selected.contains(parentItem())) {

			// delete from parents list ones that are selected right now
			QList<QGraphicsItem *> realNewParents;
			foreach (QGraphicsItem * item, parentsList){
				if (!selected.contains(item)) {
					realNewParents.append(item);
				}
			}

			NodeElement *newParent = NULL;
			int i = 0;
			// we get the first valid NodeElement from realNewParents
			while (!newParent && i < realNewParents.size()){
				newParent = dynamic_cast<NodeElement *>(realNewParents.at(i));
				i++;
			}
			// проверка, можно ли добавлять наш элемент в найденного родителя
			bool allowed = false;
			if (newParent) {
				foreach (qReal::Id type, mGraphicalAssistApi->editorManager().getContainedTypes(newParent->id().type())) {
					if (id().element() ==  type.editor()) {
						allowed = true;
					}
				}
			}
			if (newParent && !selected.contains(newParent) && allowed) {
				mGraphicalAssistApi->changeParent(id(), newParent->id(),
					mapToItem(evScene->getElem(newParent->id()), mapFromScene(scenePos())));

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
	}

	mDragState = None;
	setZValue(0);
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

void NodeElement::setVisibleEmbeddedLinkers(const bool show)
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

QVariant NodeElement::itemChange(GraphicsItemChange change, const QVariant &value)
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

const QPointF NodeElement::getNearestPort(QPointF location) const
{
	QPointF min;
	if (mPointPorts.size() > 0) {
		QPointF const pointPort = newTransform(mPointPorts[0]);
		min.setX(pointPort.x() + boundingRect().left());
		min.setY(pointPort.y() + boundingRect().top());
	}
	else if (mLinePorts.size() > 0) {
		min = mLinePorts[0].line.p1();
	} else {
		return location;
	}

	foreach (StatPoint port, mPointPorts) {
		QPointF const pointPort = newTransform(port);
		port.point.setX(pointPort.x() + boundingRect().left());
		port.point.setY(pointPort.y() + boundingRect().top());
		if (QLineF(port.point, location).length() < QLineF(min, location).length()) {
			min = port.point;
		}
	}
	if (mPointPorts.size() > 0) {
		return min;
	}

	//TODO: improve line port search
	int num = 0;
	foreach (StatLine line, mLinePorts) {
		qreal k = getNearestPointOfLinePort(num, location);
		QPointF port = QPointF((line.line.p1().x()*(1-k)+line.line.p2().x()*k),
							   (line.line.p1().y()*(1-k)+line.line.p2().y()*k));
		if (QLineF(port, location).length() < QLineF(min, location).length()) {
			min = port;
		}
		num++;
	}

	return min;
}

QLineF NodeElement::newTransform(const StatLine& port) const
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

QPointF NodeElement::newTransform(const StatPoint& port) const
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

qreal NodeElement::minDistanceFromLinePort(int linePortNumber, const QPointF &location) const
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

qreal NodeElement::distanceFromPointPort(int pointPortNumber, const QPointF &location) const
{
	return QLineF(newTransform(mPointPorts[pointPortNumber]), location).length();
}

qreal NodeElement::getNearestPointOfLinePort(int linePortNumber, const QPointF &location) const
{
	qreal nearestPointOfLinePort = 0;
	QLineF nearestLinePort = newTransform(mLinePorts[linePortNumber]);
	if (nearestLinePort.x1() == nearestLinePort.x2()) {
		nearestPointOfLinePort = (location.y() - nearestLinePort.y1())
				/ (nearestLinePort.y2() - nearestLinePort.y1());
	} else if (nearestLinePort.y1() == nearestLinePort.y2()) {
		nearestPointOfLinePort = (location.x() - nearestLinePort.x1())
				/ (nearestLinePort.x2() - nearestLinePort.x1());
	} else {
		qreal k = (nearestLinePort.y2() - nearestLinePort.y1())
				/ (nearestLinePort.x2() - nearestLinePort.x1());
		qreal b2 = location.y() + 1 / k * location.x();
		qreal b = nearestLinePort.y1() - k * nearestLinePort.x1();
		qreal x3 = k / (1 + k * k) * (b2 - b);
		nearestPointOfLinePort = (x3 - nearestLinePort.x1())
				/ (nearestLinePort.x2() - nearestLinePort.x1());
	}
	return nearestPointOfLinePort;
}

qreal NodeElement::getPortId(const QPointF &location) const
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

NodeElement *NodeElement::getNodeAt(const QPointF &position)
{
	foreach (QGraphicsItem *item, scene()->items(position)) {
		NodeElement *e = dynamic_cast<NodeElement *>(item);
		if (e && (item != this)) {
			return e;
		}
	}
	return 0;
}

void NodeElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *style, QWidget *widget)
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

void NodeElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
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
	qreal curChildY = mElementImpl->sizeOfForestalling() + 25; //25 - for container name
	qreal maxChildrenWidth = 0;

	if(mPlaceholder != NULL){
		maxChildrenWidth = mPlaceholder->rect().width();
	}

	foreach (QGraphicsItem* childItem, childItems()) {
		NodeElement* curItem = dynamic_cast<NodeElement*>(childItem);
		if (curItem) {
			if (curItem->mContents.width() > maxChildrenWidth) {
				maxChildrenWidth = curItem->mContents.width();
			}
		}
	}

	foreach (QGraphicsItem* childItem, childItems()) {
		NodeElement* curItem = dynamic_cast<NodeElement*>(childItem);
		if(childItem == mPlaceholder){
			QRectF rect(mElementImpl->sizeOfForestalling(), curChildY,
				maxChildrenWidth, mPlaceholder->rect().height());
			mPlaceholder->setRect(rect);
			curChildY += mPlaceholder->rect().height() + 10;
		}
		if (curItem) {
			if (mElementImpl->maximizesChildren()) {
				curItem->setGeometry(QRectF(mElementImpl->sizeOfForestalling(), curChildY,
											maxChildrenWidth, curItem->mContents.height()));
			} else {
				curItem->setGeometry(QRectF(mElementImpl->sizeOfForestalling(), curChildY,
											curItem->mContents.width(), curItem->mContents.height()));
			}

			curChildY += curItem->mContents.height() + mElementImpl->sizeOfChildrenForestalling() + 10;
			curItem->storeGeometry();
		}
	}
}

void NodeElement::drawPlaceholder(QGraphicsRectItem *placeholder, QGraphicsSceneDragDropEvent *event)
{
	qreal curChildY = mElementImpl->sizeOfForestalling() + 25 + scenePos().y(); //25 - for container name

	QPointF point = event->scenePos();

	// binary search? No because we need to know summary height of prev elements
	NodeElement* prevItem = NULL;
	foreach (QGraphicsItem* childItem, childItems()) {
		NodeElement *curItem = dynamic_cast<NodeElement*>(childItem);
		if(childItem == mPlaceholder){
			curChildY += mPlaceholder->rect().height();
		}

		if (curItem) {
			int height = curItem->mContents.height();
			if(curChildY + height / 2 > point.y()){
				prevItem = curItem;
				break;
			}

			curChildY += height + mElementImpl->sizeOfChildrenForestalling();
		}
	}
;
	erasePlaceholder(false);
	mPlaceholder = placeholder;
	mPlaceholder->setParentItem(this);
	if(prevItem != NULL){
		mPlaceholder->stackBefore(prevItem);
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

PossibleEdge NodeElement::toPossibleEdge(const StringPossibleEdge &strPossibleEdge)
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

void NodeElement::singleSelectionState(const bool singleSelected)
{
	initEmbeddedLinkers();
	setVisibleEmbeddedLinkers(true);
	Element::singleSelectionState(singleSelected);
}

void NodeElement::selectionState(const bool selected)
{
	Element::selectionState(selected);
}
