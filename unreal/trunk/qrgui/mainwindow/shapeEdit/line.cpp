#include "line.h"
#include <QPainter>
#include <QStyle>
#include <QStyleOptionGraphicsItem>

Line::Line(qreal x1, qreal y1, qreal x2, qreal y2, Item* parent)
	:Item(parent)
{
	mPen.setColor(Qt::green);
	mX1 = x1;
	mY1 = y1;
	mX2 = x2;
	mY2 = y2;
}

QRectF Line::boundingRect() const
{
	return QRectF(qMin(mX1, mX2), qMin(mY1, mY2), abs(mX2 - mX1), abs(mY2 - mY1));
}

void Line::drawItem(QPainter* painter)
{
	painter->drawLine(mX1, mY1, mX2, mY2);
}

void Line::drawExtractionForItem(QPainter* painter)
{
	painter->drawPoint(mX1, mY1);
	painter->drawPoint(mX2, mY2);
}

QLineF Line::line() const
{
	return QLineF(mX1, mY1, mX2, mY2);
}

QPainterPath Line::shape() const
{
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);

	QPainterPathStroker ps;
	ps.setWidth(9);

	path.moveTo(mX1, mY1);
	path.lineTo(mX2, mY2);
	path = ps.createStroke(path);

	return path;
}

void Line::changeDragState(qreal x, qreal y)
{
	if (QRectF(QPointF(mX1 + scenePos().x(), mY1 + scenePos().y()), QSizeF(0, 0)).adjusted(-5, -5, 5, 5).contains(QPointF(x, y)))
		mDragState = TopLeft;
	else if (QRectF(QPointF(mX2 + scenePos().x(), mY2 + scenePos().y()), QSizeF(0, 0)).adjusted(-5, -5, 5, 5).contains(QPointF(x, y)))
			mDragState = BottomRight;
	else
		mDragState = None;
}

void Line::resizeItem(QGraphicsSceneMouseEvent *event)
{
	qreal x = mapFromScene(event->scenePos()).x();
	qreal y = mapFromScene(event->scenePos()).y();
	if (mDragState != None)
		setFlag(QGraphicsItem::ItemIsMovable, false);
	if (mDragState == TopLeft) {
		setX1andY1(x, y);
	}
	else if (mDragState == BottomRight) {
			setX2andY2(x, y);
	}
}
