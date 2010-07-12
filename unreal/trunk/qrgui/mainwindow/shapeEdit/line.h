#pragma once

#include "item.h"

class Line : public Item
{
public:
	Line(qreal x1, qreal y1, qreal x2, qreal y2, Item* parent = 0);
	QLineF line() const;
	QPainterPath shape() const;
	virtual QRectF boundingRect() const;
	virtual void drawItem(QPainter* painter);
	virtual void drawExtractionForItem(QPainter* painter);
	virtual void changeDragState(qreal x, qreal y);
	virtual void resizeItem(QGraphicsSceneMouseEvent *event);
};
