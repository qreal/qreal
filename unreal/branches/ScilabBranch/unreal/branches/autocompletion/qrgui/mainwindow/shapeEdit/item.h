#pragma once

#include <QGraphicsItem>
#include <QPen>

class Item : public QGraphicsItem
{
public:
	Item(QGraphicsItem* parent = 0);
	virtual QRectF boundingRect() const = 0;
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	virtual void drawItem(QPainter* painter) = 0;
	virtual void drawExtractionForItem(QPainter* painter);

	QPen pen() const;
	QBrush brush() const;
	void setBrush(const QBrush& brush);
	void setPen(const QPen& pen);
	void setBottomRight(qreal x, qreal y);

protected:
	QPen mPen;
	QBrush mBrush;
	QRectF mRect;
	qreal mX1;
	qreal mY1;
	qreal mX2;
	qreal mY2;
};
