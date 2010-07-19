#pragma once

#include <QtGui/QPainter>

#include "item.h"
#include "ellipse.h"

class Arch : public Item
{
public:
	Arch(QRectF rect, int startAngle, int spanAngle, Item* parent);
	int startAngle() const;
	int spanAngle() const;
	void setStartAngle(int start);
	void setSpanAngle(int span);

	virtual QRectF boundingRect() const;
	virtual QRectF sceneBoundingRectCoord(QPointF const &topLeftPicture);
	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	virtual void drawExtractionForItem(QPainter* painter);
	virtual void drawScalingRects(QPainter* painter);
	virtual void resizeItem(QGraphicsSceneMouseEvent *event);

	virtual QPair<QDomElement, Item::DomElementTypes> generateItem(QDomDocument &document, QPointF const &topLeftPicture);

private:
	int mStartAngle;
	int mSpanAngle;
};
