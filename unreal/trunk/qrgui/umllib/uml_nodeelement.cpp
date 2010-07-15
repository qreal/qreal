#include "uml_nodeelement.h"
#include "../model/model.h"
#include "../view/editorviewscene.h"
#include "../../qrxml/editorInterface.h"

#include <QtGui/QStyle>
#include <QtGui/QStyleOptionGraphicsItem>
#include <QtGui/QMessageBox>
#include <QtGui/QTextCursor>
#include <QtGui/QToolTip>
#include <QtCore/QDebug>
#include <QtCore/QUuid>

#include <math.h>

using namespace UML;
using namespace qReal;

NodeElement::NodeElement(ElementImpl* impl)
	: mSwitchGrid(false), mSwitchGridAction("Switch on grid", this),
		mPortsVisible(false), mDragState(None), mElementImpl(impl), mIsFolded(false),
		mLeftPressed(false), mParentNodeElement(NULL), mPos(QPointF(0,0)), inHor(true)
{
	setAcceptHoverEvents(true);
	setFlag(ItemClipsChildrenToShape, false);

	mPortRenderer = new SdfRenderer();
	mRenderer = new SdfRenderer();
	ElementTitleFactory factory;
	QList<ElementTitleInterface*> titles;
	mElementImpl->init(mContents, mPointPorts, mLinePorts, factory, titles, mRenderer, mPortRenderer);
	foreach (ElementTitleInterface *titleIface, titles){
		ElementTitle *title = dynamic_cast<ElementTitle*>(titleIface);
		if (!title)
			continue;
		title->setParentItem(this);
		mTitles.append(title);
	}

	mFoldedContents = mContents;

	mSwitchGridAction.setCheckable(true);
	connect(&mSwitchGridAction, SIGNAL(toggled(bool)), this, SLOT(switchGrid(bool)));

	//resize(mContents);
}

NodeElement::~NodeElement()
{
	foreach(EdgeElement *edge, mEdgeList)
		edge->removeLink(this);
	foreach(ElementTitle *title, mTitles)
		delete title;

	delete mPortRenderer;
	delete mRenderer;
	delete mElementImpl;
}

void NodeElement::setName(QString value)
{
	QAbstractItemModel *im = const_cast<QAbstractItemModel *>(mDataIndex.model());
	im->setData(mDataIndex, value, Qt::DisplayRole);
}

void NodeElement::setGeometry(QRectF const &geom)
{
	prepareGeometryChange();
	setPos(geom.topLeft());
	if (geom.isValid())
		mContents = geom.translated(-geom.topLeft());
	mTransform.reset();
	mTransform.scale(mContents.width(), mContents.height());
	adjustLinks();

	foreach (ElementTitle *title, mTitles)
		title->setTextWidth(mContents.width() - title->pos().x());
}

void NodeElement::adjustLinks()
{
	foreach (EdgeElement *edge, mEdgeList)
		edge->adjustLink();

	foreach (QGraphicsItem *child, childItems()) {
		NodeElement *element = dynamic_cast<NodeElement*>(child);
		if (element)
			element->adjustLinks();
	}
}

void NodeElement::storeGeometry()
{
	QRectF tmp = mContents;
	model::Model *itemModel = const_cast<model::Model*>(static_cast<model::Model const *>(mDataIndex.model()));
	itemModel->setData(mDataIndex, pos(), roles::positionRole);
	itemModel->setData(mDataIndex, QPolygon(tmp.toAlignedRect()), roles::configurationRole);
}

void NodeElement::moveChildren(qreal dx, qreal dy)
{
	foreach (QGraphicsItem* childItem, childItems()) {
		NodeElement* curItem = dynamic_cast<NodeElement*>(childItem);
		if (curItem && !curItem->getPortStatus()) {
			curItem->moveBy(dx, dy);
			///returns object to the parent area
			if (curItem->pos().x() < mElementImpl->sizeOfForestalling())
				curItem->setPos(mElementImpl->sizeOfForestalling(), curItem->pos().y());
			if (curItem->pos().y() < mElementImpl->sizeOfForestalling())
				curItem->setPos(curItem->pos().x(), mElementImpl->sizeOfForestalling());
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

	if (this->mElementImpl->isSortContainer())
		sortChildren();

	if (this->mElementImpl->isMinimizingToChildren())
		newContents = QRectF(0, 0, 0, 0);

	//childrenMoving - negative shift of children from the point (SIZE_OF_FORESTALLING, SIZE_OF_FORESTALLING)
	//whatever it means :)
	QPointF childrenMoving = QPointF(0, 0);
	foreach (QGraphicsItem* childItem, childItems()) {
		NodeElement* curItem = dynamic_cast<NodeElement*>(childItem);
		if (curItem && (curItem->getPortStatus() && (newContents != mContents)))
			curItem->resizeChild(newContents, mContents);
		if (!curItem || curItem->getPortStatus())
			continue;

		QPointF curItemPos = curItem->scenePos() - scenePos();

		if (curItemPos.x() < childrenMoving.x() + mElementImpl->sizeOfForestalling())
			childrenMoving.setX(curItemPos.x() - mElementImpl->sizeOfForestalling());
		if (curItemPos.y() < childrenMoving.y() + mElementImpl->sizeOfForestalling())
			childrenMoving.setY(curItemPos.y() - mElementImpl->sizeOfForestalling());
	}
	setPos(pos() + childrenMoving);
	moveChildren(-childrenMoving);
	newContents.setTopLeft(childrenMoving);
	newContents.moveTo(0, 0);

	foreach (QGraphicsItem* childItem, childItems()) {
		NodeElement* curItem = dynamic_cast<NodeElement*>(childItem);
		if (!curItem || curItem->getPortStatus())
			continue;

		QRectF curChildItemBoundingRect = curItem->mContents;
		curChildItemBoundingRect.translate(curItem->pos());

		if (curChildItemBoundingRect.left() < newContents.left() + mElementImpl->sizeOfForestalling())
			newContents.setLeft(curChildItemBoundingRect.left() - mElementImpl->sizeOfForestalling());

		if (curChildItemBoundingRect.right() > newContents.right() - mElementImpl->sizeOfForestalling())
			newContents.setRight(curChildItemBoundingRect.right() + mElementImpl->sizeOfForestalling());

		if (curChildItemBoundingRect.top() < newContents.top() + mElementImpl->sizeOfForestalling())
			newContents.setTop(curChildItemBoundingRect.top() - mElementImpl->sizeOfForestalling());

		if (curChildItemBoundingRect.bottom() > newContents.bottom() - mElementImpl->sizeOfForestalling())
			newContents.setBottom(curChildItemBoundingRect.bottom() + mElementImpl->sizeOfForestalling());
	}

	if ((newContents.width() < objectMinSize) || (newContents.height() < objectMinSize))
		newContents = mFoldedContents;

	newContents.moveTo(pos());
	setGeometry(newContents);

	NodeElement* parItem = dynamic_cast<NodeElement*>(parentItem());
	if (parItem)
		parItem->resize(parItem->mContents); // recursive expansion of parents
}

QList<ContextMenuAction*> NodeElement::contextMenuActions()
{
	QList<ContextMenuAction*> result;
	result.push_back(&mSwitchGridAction);
	return result;
}

void NodeElement::switchOnGrid()
{
	mSwitchGrid = true;
}

void NodeElement::switchOffGrid()
{
	mSwitchGrid = false;
}

void NodeElement::delUnusedLines()
{
	for (int i = mLines.size() - 1; i >= 0; i--) {
		mLines[i]->hide();
		scene()->removeItem(mLines[i]);
		mLines.pop_back();
	}
}

//drawing a horizontal line
void NodeElement::drawLineY(qreal pointY, qreal myX)
{
	bool lineIsFound = false;
	qreal x1 = myX - widthLineX / 2;
	qreal x2 = myX + widthLineX / 2;
	if (myX - scene()->sceneRect().x() < widthLineX / 2)
		x1 = scene()->sceneRect().x() + 10;
	if (scene()->sceneRect().x() + scene()->sceneRect().width() - myX < widthLineX / 2)
		x2 = scene()->sceneRect().x() + scene()->sceneRect().width() - 10;
	QLineF line(x1, pointY, x2, pointY);

	// checking whether the scene already has this line or not.
	// if not (lineIsFound is false), then adding it
	foreach (QGraphicsLineItem* lineItem, mLines) {
		if (lineItem->line().y1() == line.y1() && lineItem->line().y2() == line.y2())
			lineIsFound = true;
	}
	if (!lineIsFound)
		mLines.push_back(scene()->addLine(line, QPen(Qt::black, 0.25, Qt::DashLine)));
}

//drawing a vertical line
void NodeElement::drawLineX(qreal pointX, qreal myY)
{
	bool lineIsFound = false;
	qreal y1 = myY - widthLineY / 2;
	qreal y2 = myY + widthLineY / 2;
	if (myY - scene()->sceneRect().y() < widthLineY / 2)
		y1 = scene()->sceneRect().y() + 10;
	if (scene()->sceneRect().y() + scene()->sceneRect().height() - myY < widthLineY / 2)
		y2 = scene()->sceneRect().y() + scene()->sceneRect().height() - 10;
	QLineF line(pointX, y1, pointX, y2);

	// checking whether the scene already has this line or not.
	// if not (lineIsFound is false), then adding it
	foreach (QGraphicsLineItem* lineItem, mLines) {
		if (lineItem->line().x1() == line.x1() && lineItem->line().x2() == line.x2())
			lineIsFound = true;
	}
	if (!lineIsFound)
		mLines.push_back(scene()->addLine(line, QPen(Qt::black, 0.25, Qt::DashLine)));
}

// checking whether we should align with the vertical line or not
bool NodeElement::makeJumpX(qreal deltaX, qreal radiusJump, qreal pointX)
{
	if (deltaX <= radiusJump) {
		setX(pointX - boundingRect().x());
		adjustLinks();
		return true;
	}
	return false;
}

// checking whether we should align with the horizontal line or not
bool NodeElement::makeJumpY(qreal deltaY, qreal radiusJump, qreal pointY)
{
	if (deltaY <= radiusJump) {
		setY(pointY - boundingRect().y());
		adjustLinks();
		return true;
	}
	return false;
}

// build a vertical line: draw it and check for alignment
void NodeElement::buildLineX(qreal deltaX, qreal radius, bool doAlways,
	qreal radiusJump, qreal pointX, qreal correctionX, qreal &myX1, qreal &myX2, qreal myY)
{
	if (deltaX <= radius || doAlways) {
		drawLineX(pointX, myY);
		if (makeJumpX(deltaX, radiusJump, pointX - correctionX)) {
			myX1 = recountX1();
			myX2 = recountX2(myX1);
		}
	}
}

// build a horizontal line: draw it and check for alignment
void NodeElement::buildLineY(qreal deltaY, qreal radius, bool doAlways,
	qreal radiusJump, qreal pointY, qreal correctionY, qreal &myY1, qreal &myY2, qreal myX)
{
	if (deltaY <= radius || doAlways) {
		drawLineY(pointY, myX);
		if (makeJumpY(deltaY, radiusJump, pointY - correctionY)) {
			myY1 = recountY1();
			myY2 = recountY2(myY1);
		}
	}
}

qreal NodeElement::recountX1()
{
	return scenePos().x() + boundingRect().x();
}

qreal NodeElement::recountX2(qreal myX1)
{
	return myX1 + boundingRect().width();
}

qreal NodeElement::recountY1()
{
	return scenePos().y() + boundingRect().y();
}

qreal NodeElement::recountY2(qreal myY1)
{
	return myY1 + boundingRect().height();
}

// move element vertically according to the grid
void NodeElement::makeGridMovingX(qreal myX, int koef, int indexGrid)
{
	int oneKoef = 0;
	if (koef != 0)
		oneKoef = koef / qAbs(koef);
	if (qAbs(qAbs(myX) - qAbs(koef) * indexGrid) <= indexGrid / 2) {
		setX(koef * indexGrid);
		adjustLinks();
	}
	else if (qAbs(qAbs(myX) - (qAbs(koef) + 1) * indexGrid) < indexGrid / 2) {
		setX((koef + oneKoef) * indexGrid);
		adjustLinks();
	}
}

// move element horizontally according to the grid
void NodeElement::makeGridMovingY(qreal myY, int koef, int indexGrid)
{
	int oneKoef = 0;
	if (koef != 0)
		oneKoef = koef / qAbs(koef);
	if (qAbs(qAbs(myY) - qAbs(koef) * indexGrid) <= indexGrid / 2) {
		setY(koef * indexGrid);
		adjustLinks();
	}
	else if (qAbs(qAbs(myY) - (qAbs(koef) + 1) * indexGrid) < indexGrid / 2) {
		setY((koef + oneKoef) * indexGrid);
		adjustLinks();
	}
}

void NodeElement::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (embeddedLinkers.isEmpty())
		initEmbeddedLinkers();
	moveEmbeddedLinkers();
	foreach(EmbeddedLinker* embeddedLinker, embeddedLinkers)
		embeddedLinker->setCovered(true);

	if (isSelected()) {
		if (QRectF(mContents.topLeft(), QSizeF(4, 4)).contains(event->pos()))
			mDragState = TopLeft;
		else if (QRectF(mContents.topRight(), QSizeF(-4, 4)).contains(event->pos()))
			mDragState = TopRight;
		else if (QRectF(mContents.bottomRight(), QSizeF(-12, -12)).contains(event->pos()))
			mDragState = BottomRight;
		else if (QRectF(mContents.bottomLeft(), QSizeF(4, -4)).contains(event->pos()))
			mDragState = BottomLeft;
		else if (QRectF(mContents.topLeft(), QSizeF(20, 20)).contains(event->pos())
				&& this->mElementImpl->isContainer())
			changeFoldState();
		else
			Element::mousePressEvent(event);
	}
	else
		Element::mousePressEvent(event);

	if (event->button() == Qt::RightButton)
		event->accept();

	mLeftPressed = true;
	setZValue(1);
}

void NodeElement::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	scene()->invalidate();
	foreach(EmbeddedLinker* embeddedLinker, embeddedLinkers)
		embeddedLinker->setCovered(false);
	if (mDragState == None) {
		Element::mouseMoveEvent(event);

		NodeElement* parItem = dynamic_cast<NodeElement*>(parentItem());
		if(parItem == NULL) {
			qreal myX1 = scenePos().x() + boundingRect().x();
			qreal myY1 = scenePos().y() + boundingRect().y();

			if (mSwitchGrid) {
				int coefX = static_cast<int>(myX1) / indexGrid;
				int coefY = static_cast<int>(myY1) / indexGrid;

				makeGridMovingX(myX1, coefX, indexGrid);
				makeGridMovingY(myY1, coefY, indexGrid);

				myX1 = scenePos().x() + boundingRect().x();
				myY1 = scenePos().y() + boundingRect().y();
			}

			qreal myX2 = myX1 + boundingRect().width();
			qreal myY2 = myY1 + boundingRect().height();

			qreal radius = 20;
			qreal radiusJump = 10;

			QList<QGraphicsItem *> list = scene()->items();
			delUnusedLines();
			foreach (QGraphicsItem *graphicsItem, list) {
				NodeElement* item = dynamic_cast<NodeElement*>(graphicsItem);
				if (item == NULL)
					continue;
				QPointF point = item->scenePos();
				qreal pointX1 = point.x() + item->boundingRect().x();
				qreal pointY1  = point.y() + item->boundingRect().y();
				qreal pointX2 = pointX1  + item->boundingRect().width();
				qreal pointY2  = pointY1 + item->boundingRect().height();

				if (pointX1 != myX1 || pointY1 != myY1) {
					qreal deltaY1 = qAbs(pointY1 - myY1);
					qreal deltaY2 = qAbs(pointY2 - myY2);
					qreal deltaX1 = qAbs(pointX1 - myX1);
					qreal deltaX2 = qAbs(pointX2 - myX2);
					if (deltaY1 <= radius || deltaY2 <= radius) {
						buildLineY(deltaY1, radius, true, radiusJump, pointY1, 0, myY1, myY2, myX1);
						buildLineY(deltaY2, radius, true, radiusJump, pointY2,
								boundingRect().height(), myY1, myY2, myX1);
					}
					if (deltaX1 <= radius || deltaX2 <= radius) {
						buildLineX(deltaX1, radius, true, radiusJump, pointX1, 0, myX1, myX2, myY1);
						buildLineX(deltaX2, radius, true, radiusJump, pointX2,
								boundingRect().width(), myX1, myX2, myY1);
					}
					buildLineY(qAbs(pointY1 - myY2), radius, false, radiusJump, pointY1,
							boundingRect().height(), myY1, myY2, myX1);
					buildLineX(qAbs(pointX1 - myX2), radius, false, radiusJump, pointX1,
							boundingRect().width(), myX1, myX2, myY1);
					buildLineY(qAbs(pointY2 - myY1), radius, false, radiusJump, pointY2,
							0, myY1, myY2, myX1);
					buildLineX(qAbs(pointX2 - myX1), radius, false, radiusJump, pointX2,
							0, myX1, myX2, myY1);
				}
			}
		}
	} else {
		QRectF newContents = mContents;

		QPointF parentPos = QPointF(0, 0);
		QGraphicsItem* parItem = parentItem();
		if (parItem)
			parentPos = parItem->scenePos();

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
	if (getPortStatus())
	{
		if (mLeftPressed)
		{
			if (mPos == QPointF(0,0))
				mPos = this->pos();
			QGraphicsItem* item = NULL;
			QPointF position = event->scenePos();
			QGraphicsScene* sc = this->scene();
			item = sc->items(position).value(1);
			NodeElement* actionItem = dynamic_cast<NodeElement*>(item);
			QPointF posInItem = QPointF(0,0);
			if (actionItem && ((actionItem == mParentNodeElement) || (!mParentNodeElement)))
			{
				if (actionItem->getHavePortStatus())
				{
					double xHor = actionItem->getXHor();
					double xVert = actionItem->getXVert();
					double yHor = actionItem->getYHor();
					double yVert = actionItem->getYVert();
					posInItem = actionItem->mapFromScene(position);
					if (actionItem->isLowSide(posInItem, xHor, yHor) || actionItem->isHighSide(posInItem, xHor, yHor)
							|| actionItem->isRightSide(posInItem, xVert, yVert) || actionItem->isLeftSide(posInItem, xVert, yVert))
					{
						this->setParentItem(actionItem);
						mParentNodeElement = actionItem;
						mPos = this->pos();
						if (actionItem->isLowSide(posInItem, xHor, yHor) || actionItem->isHighSide(posInItem, xHor, yHor))
							inHor = true;
						else
							inHor = false;
					}
					else
					{
						if (inHor)
						{
							if (actionItem->isNoBorderY(posInItem, xHor, yHor))
							{
								double x = posInItem.x();
								double y = mPos.y();
								mPos = QPointF(x,y);
								setPos(mPos);
							}
							else
								setPos(mPos);
						}
						else
						{
							if (actionItem->isNoBorderX(posInItem, xVert, yVert))
							{
								double x = mPos.x();
								double y = posInItem.y();
								mPos = QPointF(x,y);
								setPos(mPos);
							}
							else
								setPos(mPos);
						}
					}
				}
			}
			else
			{
				if ((mParentNodeElement) && (mParentNodeElement->getHavePortStatus()))
				{
					setPos(mPos);
					if (mParentNodeElement)
					{
						posInItem = mParentNodeElement->mapFromScene(position);
						double xHor = mParentNodeElement->getXHor();
						double xVert = mParentNodeElement->getXVert();
						double yHor = mParentNodeElement->getYHor();
						double yVert = mParentNodeElement->getYVert();
						if (inHor)
						{
							if (mParentNodeElement->isNoBorderY(posInItem, xHor, yHor))
							{
								double x = posInItem.x();
								double y = mPos.y();
								mPos = QPointF(x,y);
								setPos(mPos);
							}
							else
								setPos(mPos);
						}
						else
						{
							if (mParentNodeElement->isNoBorderX(posInItem, xVert, yVert))
							{
								double x = mPos.x();
								double y = posInItem.y();
								mPos = QPointF(x,y);
								setPos(mPos);
							}
							else
								setPos(mPos);
						}
					}
				}
			}
		}
	}
}

void NodeElement::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	delUnusedLines();
	mContents = mContents.normalized();
	storeGeometry();

	moveEmbeddedLinkers();
		foreach(EmbeddedLinker* embeddedLinker, embeddedLinkers)
			embeddedLinker->setCovered(true);

	if (mDragState == None)
		Element::mouseReleaseEvent(event);

	if (!getPortStatus() && (flags() & ItemIsMovable))
	{
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

		NodeElement *newParent = getNodeAt(newParentInnerPoint);

		EditorViewScene *evScene = dynamic_cast<EditorViewScene *>(scene());
		model::Model *itemModel = const_cast<model::Model*>(static_cast<const model::Model*>(mDataIndex.model()));
		if (newParent) {
			itemModel->changeParent(mDataIndex, newParent->mDataIndex,
					mapToItem(evScene->getElemByModelIndex(newParent->mDataIndex), mapFromScene(scenePos())));

			newParent->resize(newParent->mContents);

			while (newParent) {
				newParent->mContents = newParent->mContents.normalized();
				newParent->storeGeometry();
				newParent = dynamic_cast<NodeElement*>(newParent->parentItem());
			}
		} else
			itemModel->changeParent(mDataIndex, evScene->rootItem(), scenePos());
	}

	mDragState = None;
	setZValue(0);
}

void NodeElement::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	QList<QGraphicsItem*> graphicsItems = scene()->selectedItems();
	int length = graphicsItems.size();
	foreach(QGraphicsItem* item, graphicsItems) {
		UML::EdgeElement* edge = dynamic_cast<UML::EdgeElement*>(item);
		if (edge) {
		length--;
		graphicsItems.removeOne(edge);
		}
	}

	if (length > 1) {
		foreach(QGraphicsItem* item, scene()->selectedItems()) {
		UML::NodeElement* node = dynamic_cast<UML::NodeElement*>(item);
		if (node)
			node->hideEmbeddedLinkers();
		}
	}
		Q_UNUSED(event);
	if (!isSelected())
		return;

		if (embeddedLinkers.isEmpty())
			initEmbeddedLinkers();
		foreach(EmbeddedLinker* embeddedLinker, embeddedLinkers)
			embeddedLinker->setCovered(true);
}

void NodeElement::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
		Q_UNUSED(event);
}

void NodeElement::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	Q_UNUSED(event);
	if (!isSelected())
		return;
//        foreach(EmbeddedLinker* mEmbeddedLinker, embeddedLinkers)
//            mEmbeddedLinker->setCovered(false);
}

void NodeElement::hideEmbeddedLinkers()
{
	foreach(EmbeddedLinker* embeddedLinker, embeddedLinkers)
		embeddedLinker->setCovered(false);
}

bool NodeElement::initPossibleEdges()
{
	if (!possibleEdges.isEmpty())
		return true;
	model::Model* itemModel = const_cast<model::Model*>(static_cast<const model::Model*>(mDataIndex.model()));
	if (!itemModel)
		return false;

	foreach(QString elementName,
	itemModel->assistApi().editorManager().getEditorInterface(this->uuid().editor())->elements(this->uuid().diagram())) {
		int ne = itemModel->assistApi().editorManager().getEditorInterface(this->uuid().editor())->isNodeOrEdge(elementName);
		if (ne == -1) {
			QList<PossibleEdge> list
			= itemModel->assistApi().editorManager().getEditorInterface(this->uuid().editor())->getPossibleEdges(elementName);
			foreach(PossibleEdge pEdge, list) {
				if ((pEdge.first.first == this->uuid().element())
				|| ((pEdge.first.second == this->uuid().element()) && (!pEdge.second.first))) {
					possibleEdges.insert(pEdge);
					possibleEdgeTypes.insert(pEdge.second);
				}
			}
		}
	}

	return (!possibleEdges.isEmpty());
}

bool NodeElement::initEmbeddedLinkers()
{
	int counter = 0;
	typedef QPair<bool,QString> Pair;
	QSet<QString> usedEdges;
	foreach(Pair type, possibleEdgeTypes) {
		if (usedEdges.contains(type.second))
			continue;
		EmbeddedLinker* embeddedLinker = new EmbeddedLinker();
		scene()->addItem(embeddedLinker);
		embeddedLinker->setEdgeType(type.second);
		embeddedLinker->setDirected(type.first);
		embeddedLinkers.append(embeddedLinker);
		embeddedLinker->setMaster(this);
		usedEdges.insert(type.second);
		counter++;
	}
	moveEmbeddedLinkers();
	foreach(EmbeddedLinker* embeddedLinker, embeddedLinkers)
		embeddedLinker->initTitle();

	return (counter > 0);
}

void NodeElement::moveEmbeddedLinkers()
{
	int index = 0;
	int maxIndex = embeddedLinkers.size();
	foreach(EmbeddedLinker* embeddedLinker, embeddedLinkers)
	{
		embeddedLinker->takePosition(index,maxIndex);
	index++;
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
		QPointF newpos = mDataIndex.data(roles::positionRole).toPointF();
		QPolygon newpoly = mDataIndex.data(roles::configurationRole).value<QPolygon>();
		QRectF newRect; // Use default ((0,0)-(0,0))
		// QPolygon::boundingRect is buggy :-(
		if (!newpoly.isEmpty()) {
			int minx = newpoly[0].x();
			int miny = newpoly[0].y();
			int maxx = newpoly[0].x();
			int maxy = newpoly[0].y();;
			for (int i = 1; i < newpoly.size(); ++i) {
				if (minx > newpoly[i].x())
					minx = newpoly[i].x();
				if (maxx < newpoly[i].x())
					maxx = newpoly[i].x();
				if (miny > newpoly[i].y())
					miny = newpoly[i].y();
				if (maxy < newpoly[i].y())
					maxy = newpoly[i].y();
			}
			newRect = QRectF(QPoint(minx, miny), QSize(maxx - minx, maxy - miny));
		}
		setGeometry(newRect.translated(newpos));
	}

	mElementImpl->updateData(this);
	update();
}

static int portId(qreal id)
{
	int iid = qRound(id);
	if (id < 1.0 * iid)
		return iid - 1;
	else
		return iid;
}

const QPointF NodeElement::getPortPos(qreal id) const
{
	int iid = portId(id);

	if (id < 0.0)
		return QPointF(0, 0);
	if (id < mPointPorts.size())
		return mTransform.map(mPointPorts[iid]);
	if (id < mPointPorts.size() + mLinePorts.size())
		return newTransform(mLinePorts.at(iid - mPointPorts.size())).pointAt(id - 1.0 * iid);
	else
		return QPointF(0, 0);
}

const QPointF NodeElement::getNearestPort(QPointF location) const
{
	int num = 0;
	QPointF min;
	if (mPointPorts.size() > 0)
		min = mPointPorts[0];
	else if (mLinePorts.size() > 0)
		min = mLinePorts[0].line.p1();
	else
		return location;

	foreach (QPointF port, mPointPorts) {
		qreal minDistance = sqrt(pow((min.x()-location.x()),2) + pow((min.y()-location.y()),2));
		if (distanceFromPointPort(num, location) < minDistance)
			min = port;
		num++;
	}
	if (num > 0)
		return min;

	num = 0;
	foreach (StatLine line, mLinePorts) {
		qreal k = getNearestPointOfLinePort(num, location);
		QPointF port = QPointF((line.line.p1().x()*(1-k)+line.line.p2().x()*k),
			(line.line.p1().y()*(1-k)+line.line.p2().y()*k));
		qreal minDistance = sqrt(pow((min.x()-location.x()),2) + pow((min.y()-location.y()),2));
		qreal distance = sqrt(pow((port.x()-location.x()),2) + pow((port.y()-location.y()),2));
		if (distance < minDistance)
			min = port;
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

	if (port.prop_x1)
		x1 = port.line.x1() * 100;
	else
		x1 = port.line.x1() * contentsRect().width();

	if (port.prop_y1)
		y1 = port.line.y1() * 100;
	else
		y1 = port.line.y1() * contentsRect().height();

	if (port.prop_x2)
		x2 = port.line.x2() * 100;
	else
		x2 = port.line.x2() * contentsRect().width();

	if (port.prop_y2)
		y2 = port.line.y2() * 100;
	else
		y2 = port.line.y2() * contentsRect().height();

	return QLineF(x1, y1, x2, y2);
}

qreal NodeElement::minDistanceFromLinePort(int linePortNumber, const QPointF &location) const
{
	QLineF linePort = newTransform(mLinePorts[linePortNumber]);
	qreal a = linePort.length();
	qreal b = QLineF(linePort.p1(), location).length();
	qreal c = QLineF(linePort.p2(), location).length();

	qreal nearestPointOfLinePort = getNearestPointOfLinePort(linePortNumber, location);
	if ((nearestPointOfLinePort < 0) || (nearestPointOfLinePort > 0.9999))
		return qMin(b, c);
	else {
		qreal p = (a + b + c) / 2;
		qreal triangleSquare = sqrt(p * (p - a) * (p - b) * (p - c));
		qreal minDistance = 2 * triangleSquare / a;
		return minDistance;
	}
}

qreal NodeElement::distanceFromPointPort(int pointPortNumber, const QPointF &location) const
{
	return QLineF(mTransform.map(mPointPorts[pointPortNumber]), location).length();
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
		if (QRectF(mTransform.map(mPointPorts[i]) - QPointF(kvadratik, kvadratik),
			QSizeF(kvadratik * 2, kvadratik * 2)).contains(location))
		{
			return 1.0 * i;
		}
	}

	for (int i = 0; i < mLinePorts.size(); i++) {
		QPainterPathStroker ps;
		ps.setWidth(kvadratik);

		QPainterPath path;
		path.moveTo(newTransform(mLinePorts[i]).p1());
		path.lineTo(newTransform(mLinePorts[i]).p2());

		path = ps.createStroke(path);
		if (path.contains(location))
			return (1.0 * (i + mPointPorts.size()) + qMin(0.9999,
				QLineF(QLineF(newTransform(mLinePorts[i])).p1(), location).length()
				/ newTransform(mLinePorts[i]).length()));
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
			if (nearestPointOfLinePort < 0)
				nearestPointOfLinePort = 0;
			else if (nearestPointOfLinePort > 0.9999)
				nearestPointOfLinePort = 0.9999;
			return 1.0 * (numMinDistance + nearestPointOfLinePort + mPointPorts.size());
		} else
			return 1.0 * numMinDistance;
	} else if (numMinDistance >= 0)
		return 1.0 * numMinDistance;
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
		if (e && (item != this))
			return e;
	}
	return 0;
}

void NodeElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *style, QWidget *widget)
{
	mElementImpl->paint(painter, mContents);
	if (mElementImpl->hasPorts())
		paint(painter, style, widget, mPortRenderer);
	else
		paint(painter, style, widget, 0);
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

			if (mElementImpl->isContainer())
				painter->drawRect(QRectF(mContents.topLeft(), QSizeF(20, 20)));

			b.setColor(Qt::blue);
			painter->setBrush(b);
			painter->setPen(Qt::blue);

			painter->drawRect(QRectF(mContents.topLeft(), QSizeF(4, 4)));
			painter->drawRect(QRectF(mContents.topRight(), QSizeF(-4, 4)));
			painter->drawRect(QRectF(mContents.bottomLeft(), QSizeF(4, -4)));

			painter->translate(mContents.bottomRight());
			painter->drawLine(QLineF(-4, 0, 0, -4));
			painter->drawLine(QLineF(-8, 0, 0, -8));
			painter->drawLine(QLineF(-12, 0, 0, -12));

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

void NodeElement::switchGrid(bool isChecked)
{
	mSwitchGrid = isChecked;
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

	foreach (QGraphicsItem* childItem, childItems()) {
		NodeElement* curItem = dynamic_cast<NodeElement*>(childItem);
		if (curItem) {
			QRectF curChildContents = curItem->mContents;
			curChildContents.moveTo(mElementImpl->sizeOfForestalling(), curChildY);
			curItem->setGeometry(curChildContents);
			curChildY += curItem->mContents.height() + mElementImpl->sizeOfChildrenForestalling();
			curItem->storeGeometry();

			if (curItem->mContents.width() > maxChildrenWidth)
				maxChildrenWidth = curItem->mContents.width();
		}
	}

	/*
	QRectF newContents(pos(), maxChildrenWidth + 2 * mElementImpl->sizeOfForestalling(), curChildPosition.y() + mElementImpl->sizeOfForestalling());
	setGeometry(newContents);
	*/
}

bool NodeElement::getPortStatus()
{
	return mElementImpl->isPort();
}

bool NodeElement::getHavePortStatus()
{
	return mElementImpl->isHavePin();
}

double NodeElement::getXHor()
{
	return mElementImpl->getXHorBord();
}

double NodeElement::getYHor()
{
	return mElementImpl->getYHorBord();
}

double NodeElement::getXVert()
{
	return mElementImpl->getXVertBord();
}

double NodeElement::getYVert()
{
	return mElementImpl->getYVertBord();
}

bool NodeElement::isLowSide(QPointF& point, double x, double y) const
{
	double m_x = point.rx();
	double m_y = point.ry();
	QRectF rc = boundingRect();
	return (m_x >= rc.x() + x) && (m_x <= rc.x() + rc.width() - x) && (m_y >= rc.y() + rc.height() - y)
			&& (m_y <= rc.y() + rc.height() + y);
}

bool NodeElement::isHighSide(QPointF& point, double x, double y) const
{
	double m_x = point.rx();
	double m_y = point.ry();
	QRectF rc = boundingRect();
	return (m_x >= rc.x() + x) && (m_x <= rc.x() + rc.width() - x) && (m_y >= rc.y() - y)
			&& (m_y <= rc.y() + y);
}

bool NodeElement::isLeftSide(QPointF& point, double x, double y) const
{
	double m_x = point.rx();
	double m_y = point.ry();
	QRectF rc = boundingRect();
	return (m_x >= rc.x() - x) && (m_x <= rc.x() + x) && (m_y >= rc.y() + y)
			&& (m_y <= rc.y() + rc.height() - y);
}

bool NodeElement::isRightSide(QPointF& point, double x, double y) const
{
	double m_x = point.rx();
	double m_y = point.ry();
	QRectF rc = boundingRect();
	return (m_x >= rc.x() + rc.width() - x) && (m_x <= rc.x() + rc.width() + x) && (m_y >= rc.y() + y)
			&& (m_y <= rc.y() + rc.height() - y);
}

bool NodeElement::isNoBorderX(QPointF& point, double x, double y) const
{
	Q_UNUSED(x);
	double m_y = point.ry();
	QRectF rc = boundingRect();
	return (m_y >= rc.y() + y) && (m_y <= rc.y() + rc.height() - y);
}

bool NodeElement::isNoBorderY(QPointF& point, double x, double y) const
{
	Q_UNUSED(y);
	double m_x = point.rx();
	QRectF rc = boundingRect();
	return (m_x >= rc.x() + x) && (m_x <= rc.x() + rc.width() - x);
}

void NodeElement::resizeChild(QRectF newContents, QRectF oldContents)
{
	if (!mParentNodeElement)
	{
		QGraphicsItem* item = parentItem();
		mParentNodeElement = dynamic_cast<NodeElement*>(item);
	}
	if (mPos == QPointF(0,0))
		mPos = this->pos();
	double xHor = mParentNodeElement->getXHor();
	double xVert = mParentNodeElement->getXVert();
	double yHor = mParentNodeElement->getYHor();
	double yVert = mParentNodeElement->getYVert();
	QPointF posi = pos();
	if (mParentNodeElement->isLowSide(posi, xHor, yHor+5))
	{
		double x = mPos.x() - oldContents.x();
		double a = oldContents.x() + oldContents.width();
		double b = newContents.x() + newContents.width();
		double dy = newContents.height() - oldContents.height();
		mPos = QPointF(newContents.x() + x*b/a, mPos.y()+dy);
		setPos(mPos);
	}
	if (mParentNodeElement->isHighSide(posi, xHor, yHor))
	{
		double x = mPos.x() - oldContents.x();
		double a = oldContents.x() + oldContents.width();
		double b = newContents.x() + newContents.width();
		double dy = 0;
		mPos = QPointF(newContents.x() + x*b/a, mPos.y()+dy);
		setPos(mPos);
	}
	if (mParentNodeElement->isRightSide(posi, xVert+5, yVert))
	{
		double y = mPos.y() - oldContents.y();
		double a = oldContents.y() + oldContents.height();
		double b = newContents.y() + newContents.height();
		double dx = newContents.width() - oldContents.width();
		mPos = QPointF(mPos.x()+dx, newContents.y() + y*b/a);
		setPos(mPos);
	}
	if (mParentNodeElement->isLeftSide(posi, xVert, yVert))
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
	resize(mContents);
}

void NodeElement::updateByNewParent()
{
	NodeElement* parent = dynamic_cast<NodeElement*>(parentItem());
	if (!parent || parent->mElementImpl->isChildrenMovable())
		setFlag(ItemIsMovable, true);
	else
		setFlag(ItemIsMovable, false);
}

bool NodeElement::isClass()
{
	return mElementImpl->isClass();
}
