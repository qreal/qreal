#pragma once

#include "item.h"

class Line : public Item
{
public:
	Line(qreal x1, qreal y1, qreal x2, qreal y2, Item* parent = 0);
	Line(Line const &other);
	virtual Item* clone();
	QLineF line() const;
	QPainterPath shape() const;
	virtual QRectF boundingRect() const;
	virtual QRectF realBoundingRect() const;
	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	virtual void drawExtractionForItem(QPainter* painter);
	virtual void drawScalingRects(QPainter* painter);
	virtual void drawFieldForResizeItem(QPainter* painter);
	virtual void changeScalingPointState(qreal x, qreal y);
	virtual void resizeItem(QGraphicsSceneMouseEvent *event);
	virtual void reshapeRectWithShift();

	QPair<QPair<QString, QString>, QPair<QString, QString> > setXandYBefore(QRectF const &rect);
	void setXandY(QDomElement& dom, QPair<QPair<QString, QString>, QPair<QString, QString> > pair);
	virtual QPair<QDomElement, Item::DomElementTypes> generateItem(QDomDocument &document, QPoint const &topLeftPicture);
};
