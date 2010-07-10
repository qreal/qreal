#pragma once

#include <QtGui/QGraphicsItem>
#include <QtGui/QPen>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QPainter>

class Item : public QGraphicsItem
{
public:
	Item(QGraphicsItem* parent = 0);
	virtual QRectF boundingRect() const = 0;
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	virtual void drawItem(QPainter* painter) = 0;
	virtual void drawExtractionForItem(QPainter* painter);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

	QPen pen() const;
	QBrush brush() const;
	void setBrush(const QBrush& brush);
	void setPen(const QPen& pen);
	void setBottomRight(qreal x, qreal y);
	void reshapeRectWithShift();

protected:
	QPen mPen;
	QBrush mBrush;
	QRectF mRect;
	qreal mX1;
	qreal mY1;
	qreal mX2;
	qreal mY2;
};
