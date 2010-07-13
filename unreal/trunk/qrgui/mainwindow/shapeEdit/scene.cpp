#include "scene.h"

#include <QtCore/QPointF>

Scene::Scene(QObject * parent)
	:  QGraphicsScene(parent), mItemType(none), mWaitMove(false), mCount(0), mGraphicsItem(NULL)
{
	setItemIndexMethod(NoIndex);
	mEmptyRect = addRect(0, 0, 710, 600, QPen(Qt::white));
}

void Scene::setX1andY1(QGraphicsSceneMouseEvent *event)
{
	mX1 = event->scenePos().x();
	mY1 = event->scenePos().y();
}

void Scene::setX2andY2(QGraphicsSceneMouseEvent *event)
{
	mX2 = event->scenePos().x();
	mY2 = event->scenePos().y();
}

void Scene::reshapeLine(QGraphicsSceneMouseEvent *event)
{
	setX2andY2(event);
	mLine->setX2andY2(mX2, mY2);
}

void Scene::reshapeLinePort(QGraphicsSceneMouseEvent *event)
{
	setX2andY2(event);
	mLinePort->setX2andY2(mX2, mY2);
}

void Scene::reshapeEllipse(QGraphicsSceneMouseEvent *event)
{
	setX2andY2(event);
	mEllipse->setX2andY2(mX2, mY2);
	if (event->modifiers() & Qt::ShiftModifier)
		mEllipse->reshapeRectWithShift();
}

void Scene::reshapeRectangle(QGraphicsSceneMouseEvent *event)
{
	setX2andY2(event);
	mRectangle->setX2andY2(mX2, mY2);
	if (event->modifiers() & Qt::ShiftModifier)
		mRectangle->reshapeRectWithShift();
}

void Scene::reshapeItem(QGraphicsSceneMouseEvent *event)
{
	setX2andY2(event);
	if (mGraphicsItem != NULL)
		mGraphicsItem->resizeItem(event);
}

void Scene::removeMoveFlag(QGraphicsSceneMouseEvent *event, QGraphicsItem* item)
{
	QList<QGraphicsItem *> list = items(event->scenePos());
	foreach (QGraphicsItem *graphicsItem, list) {
		Item *item = dynamic_cast<Item *>(graphicsItem);
		if (item != NULL)
			item->setFlag(QGraphicsItem::ItemIsMovable, false);
	}
	if (item != mEmptyRect)
		item->setFlag(QGraphicsItem::ItemIsMovable, true);
}

void Scene::setMoveFlag(QGraphicsSceneMouseEvent *event)
{
	QList<QGraphicsItem *> list = items(event->scenePos());
	foreach (QGraphicsItem *graphicsItem, list){
		Item *item = dynamic_cast<Item *>(graphicsItem);
		if (item != NULL)
			graphicsItem->setFlag(QGraphicsItem::ItemIsMovable, true);
	}
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mousePressEvent(event);
	switch (mItemType) {
	case line :
		setX1andY1(event);
		mLine = new Line(mX1, mY1, mX1, mY1, NULL);
		addItem(mLine);
		removeMoveFlag(event, mLine);
		mWaitMove = true;
		break;
	case ellipse :
		setX1andY1(event);
		mEllipse = new Ellipse(mX1, mY1, mX1, mY1, NULL);
		addItem(mEllipse);
		removeMoveFlag(event, mEllipse);
		mWaitMove = true;
		break;
	case rectangle :
		setX1andY1(event);
		mRectangle = new Rectangle(mX1, mY1, mX1, mY1, NULL);
		addItem(mRectangle);
		removeMoveFlag(event, mRectangle);
		mWaitMove = true;
		break;
	case text:
		setX1andY1(event);
		mText = new Text(mX1, mY1, "text", false);
		addItem(mText);
		break;
	case dynamicText :
		setX1andY1(event);
		mText = new Text(mX1, mY1, "name", true);
		addItem(mText);
		break;
	case pointPort :
		setX1andY1(event);
		mPointPort = new PointPort(mX1, mY1, NULL);
		addItem(mPointPort);
		removeMoveFlag(event, mPointPort);
		break;
	case linePort :
		setX1andY1(event);
		mLinePort = new LinePort(mX1, mY1, mX1, mY1, NULL);
		addItem(mLinePort);
		removeMoveFlag(event, mLinePort);
		mWaitMove = true;
		break;
	default:  // if we wait some resize
		setX1andY1(event);
		mGraphicsItem = dynamic_cast<Item *>(itemAt(event->scenePos()));
		if (mGraphicsItem != NULL) {
			mGraphicsItem->changeDragState(mX1, mY1);
		}
		break;
	}
}

void Scene::mouseMoveEvent( QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mouseMoveEvent(event);
	switch (mItemType) {
	case line :
		if (mWaitMove)
			reshapeLine(event);
		break;
	case ellipse :
		if (mWaitMove)
			reshapeEllipse(event);
		break;
	case rectangle :
		if (mWaitMove)
			reshapeRectangle(event);
		break;
	case linePort :
		if (mWaitMove)
			reshapeLinePort(event);
		break;
	default:  // if we wait some resize
		reshapeItem(event);
		break;
	}
	update();
}

void Scene::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
	QGraphicsScene::mouseReleaseEvent(event);
	switch (mItemType) {
	case line :
		reshapeLine(event);
		break;
	case ellipse :
		reshapeEllipse(event);
		break;
	case rectangle :
		reshapeRectangle(event);
		break;
	case linePort :
		reshapeLinePort(event);
		break;
	default:  // if we wait some resize
		reshapeItem(event);
		break;
	}
	mItemType = none;
	mWaitMove = false;
	mGraphicsItem = NULL;
	setMoveFlag(event);
}

void Scene::drawLine()
{
	mItemType = line;
}

void Scene::drawEllipse()
{
	mItemType = ellipse;
}

void Scene::drawArc()
{
	mItemType = arch;
	mCount = 1;
}

void Scene::drawRectangle()
{
	mItemType = rectangle;
}

void Scene::addText()
{
	mItemType = text;
}

void Scene::addDynamicText()
{
	mItemType = dynamicText;
}

void Scene::addPointPort()
{
	mItemType = pointPort;
}

void Scene::addLinePort()
{
	mItemType = linePort;
}

void Scene::deleteItem()
{
	QList<QGraphicsItem *> list = selectedItems();
	foreach (QGraphicsItem *graphicsItem, list) {
		removeItem(graphicsItem);
		delete graphicsItem;
	}
}

void Scene::clearScene()
{
	clear();
}
