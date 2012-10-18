#include "wallItem.h"
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QStyleOptionGraphicsItem>
#include <QDebug>

using namespace qReal::interpreters::robots;
using namespace details::d2Model;
using namespace graphicsUtils;

WallItem::WallItem(QPointF const &begin, QPointF const &end)
	: LineItem(begin, end)
	, mDragged(false)
	, mImage(QImage(":/icons/wall.png"))
{
	setPrivateData();
}

void WallItem::setPrivateData()
{
	setZValue(1);

	mPen.setWidth(10);

	mBrush.setStyle(Qt::SolidPattern);
	mBrush.setTextureImage(mImage);
	mPen.setStyle(Qt::NoPen);
}

QPointF WallItem::begin()
{
	return QPointF(mX1, mY1) + scenePos();
}

QPointF WallItem::end()
{
	return QPointF(mX2, mY2) + scenePos();
}

void WallItem::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->drawPath(shape());
}

void WallItem::drawExtractionForItem(QPainter *painter)
{
	if (!isSelected())
		return;

	painter->setPen(QPen(Qt::green));
	mLineImpl.drawExtractionForItem(painter, mX1, mY1, mX2, mY2, drift);
	mLineImpl.drawFieldForResizeItem(painter, resizeDrift, mX1, mY1, mX2, mY2);
}

void WallItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	AbstractItem::mousePressEvent(event);
	mDragged = true;
}

void WallItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	QPointF oldPos = pos();
	QGraphicsItem::mouseMoveEvent(event);
	if (mDragged) {
		emit wallDragged(this->realBoundingRect(), oldPos);
	}
}

bool WallItem::isDragged()
{
	return mDragged;
}

void WallItem::toStopWall(bool isNeedStop, QPointF const& oldPos)
{
	if (mDragged) {
		if (isNeedStop) {
			setPos(oldPos);
			setFlag(ItemIsMovable, false);
		} else {
			setFlag(ItemIsMovable, true);
		}
	}
}

void WallItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseReleaseEvent(event);
	mDragged = false;
}
