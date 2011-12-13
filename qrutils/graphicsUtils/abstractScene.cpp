#include "abstractScene.h"
#include "abstractItem.h"
#include <QtGui/QGraphicsSceneMouseEvent>

using namespace graphicsUtils;

AbstractScene::AbstractScene(AbstractView *view, QObject *parent) :
	QGraphicsScene(parent), mGraphicsItem(NULL)
{
	mView = view;
	mFirstPenWidth = 0;
}

void AbstractScene::setEmptyRect(int x, int y, int w, int h)
{
	mEmptyRect = addRect(x, y, w, h, QPen(Qt::white));
}

QRect AbstractScene::realItemsBoundingRect() const
{
	QRectF rect = itemsBoundingRect();
	int maxX = static_cast<int>(rect.left());
	int maxY = static_cast<int>(rect.top());
	int minY = static_cast<int>(rect.bottom());
	int minX = static_cast<int>(rect.right());
	QList<QGraphicsItem *> list = items();
	foreach (QGraphicsItem *graphicsItem, list) {

		AbstractItem* item = dynamic_cast<AbstractItem*>(graphicsItem);
		if (item != NULL) {
			QRectF itemRect = item->realBoundingRect();
			maxX = qMax(static_cast<int>(itemRect.right()), maxX);
			maxY = qMax(static_cast<int>(itemRect.bottom()), maxY);
			minX = qMin(static_cast<int>(itemRect.left()), minX);
			minY = qMin(static_cast<int>(itemRect.top()), minY);
		}
	}
	return QRect(minX, minY, maxX - minX, maxY - minY);
}

void AbstractScene::setX1andY1(QGraphicsSceneMouseEvent *event)
{
	mX1 = event->scenePos().x();
	mY1 = event->scenePos().y();
}

void AbstractScene::setX2andY2(QGraphicsSceneMouseEvent *event)
{
	mX2 = event->scenePos().x();
	mY2 = event->scenePos().y();
}

void AbstractScene::reshapeItem(QGraphicsSceneMouseEvent *event)
{
	setX2andY2(event);
	if (mGraphicsItem != NULL) {
		if (mGraphicsItem->getDragState() != graphicsUtils::AbstractItem::None)
			mView->setDragMode(QGraphicsView::NoDrag);
		mGraphicsItem->resizeItem(event);
	}
}

void AbstractScene::reshapeItem(QGraphicsSceneMouseEvent *event,graphicsUtils::AbstractItem *item)
{
	setX2andY2(event);
	if (item != NULL) {
		if (item->getDragState() != graphicsUtils::AbstractItem::None)
			mView->setDragMode(QGraphicsView::NoDrag);
		item->resizeItem(event);
	}
}

void AbstractScene::setMoveFlag(QGraphicsSceneMouseEvent *event)
{
	QList<QGraphicsItem *> list = items(event->scenePos());
	foreach (QGraphicsItem *graphicsItem, list){
		AbstractItem *item = dynamic_cast<graphicsUtils::AbstractItem *>(graphicsItem);
		if (item != NULL)
			graphicsItem->setFlag(QGraphicsItem::ItemIsMovable, true);
	}
}

void AbstractScene::removeMoveFlag(QGraphicsSceneMouseEvent *event, QGraphicsItem* item)
{
	QList<QGraphicsItem *> list = items(event->scenePos());
	foreach (QGraphicsItem *graphicsItem, list) {
		AbstractItem *grItem = dynamic_cast<graphicsUtils::AbstractItem *>(graphicsItem);
		if (grItem != NULL)
			grItem->setFlag(QGraphicsItem::ItemIsMovable, false);
	}
	if (item!= NULL && item != mEmptyRect)
		item->setFlag(QGraphicsItem::ItemIsMovable, true);
}

void AbstractScene::setDragMode(int itemsType)
{
	if (itemsType != 0) {
		mView->setDragMode(QGraphicsView::NoDrag);
	}
	else
		mView->setDragMode(QGraphicsView::RubberBandDrag);
}

void AbstractScene::forPressResize(QGraphicsSceneMouseEvent *event)
{
	setX1andY1(event);
	mGraphicsItem = dynamic_cast<AbstractItem *>(itemAt(event->scenePos()));
	if (mGraphicsItem != NULL) {
		mGraphicsItem->changeDragState(mX1, mY1);
		if (mGraphicsItem->getDragState() != AbstractItem::None)
			mView->setDragMode(QGraphicsView::NoDrag);
	}
	update();
}

void AbstractScene::forMoveResize(QGraphicsSceneMouseEvent *event)
{
	reshapeItem(event);
	update();
}

void AbstractScene::forReleaseResize(QGraphicsSceneMouseEvent * event )
{
	reshapeItem(event);
	mGraphicsItem = NULL;
	update();
}

bool AbstractScene::compareItems(AbstractItem* first, AbstractItem* second)
{
	return first->zValue() < second->zValue();
}

void AbstractScene::setEmptyPenBrushItems()
{
	mPenStyleItems = "Solid";
	mPenWidthItems = mFirstPenWidth;
	mPenColorItems = "black";
	mBrushStyleItems = "None";
	mBrushColorItems = "white";
}

void AbstractScene::setPenBrushItems(QPen const &pen, QBrush const &brush)
{
	mPenStyleItems = convertPenToString(pen);
	mPenWidthItems = pen.width();
	mPenColorItems = pen.color().name();
	mBrushStyleItems = convertBrushToString(brush);
	mBrushColorItems = brush.color().name();
}

QString AbstractScene::convertPenToString(QPen const &pen)
{
	QString penStyle;
	switch (pen.style()) {
	case Qt::SolidLine:
		penStyle = "Solid";
		break;
	case Qt::DotLine:
		penStyle = "Dot";
		break;
	case Qt::DashLine:
		penStyle = "Dash";
		break;
	case Qt::DashDotLine:
		penStyle =  "DashDot";
		break;
	case Qt::DashDotDotLine:
		penStyle = "DashDotDot";
		break;
	case Qt::NoPen:
		penStyle = "None";
		break;
	default:
		break;
	}
	return penStyle;
}

QString AbstractScene::convertBrushToString(QBrush const &brush)
{
	QString brushStyle;
	if (brush.style() == Qt::NoBrush)
		brushStyle = "None";
	if (brush.style() == Qt::SolidPattern)
		brushStyle = "Solid";
	return brushStyle;
}

QString AbstractScene::penStyleItems()
{
	return mPenStyleItems;
}

int AbstractScene::penWidthItems()
{
	return mPenWidthItems;
}

QString AbstractScene::penColorItems()
{
	return mPenColorItems;
}

QString AbstractScene::brushStyleItems()
{
	return mBrushStyleItems;
}

QString AbstractScene::brushColorItems()
{
	return mBrushColorItems;
}

void AbstractScene::setPenStyleItems(QString const &text)
{
	mPenStyleItems = text;
}

void AbstractScene::setPenWidthItems(int width)
{
	mPenWidthItems = width;
}

void AbstractScene::setPenColorItems(QString const &text)
{
	mPenColorItems = text;
}

void AbstractScene::setBrushStyleItems(QString const &text)
{
	mBrushStyleItems = text;
}

void AbstractScene::setBrushColorItems(QString const &text)
{
	mBrushColorItems = text;
}
