#pragma once
#include "item.h"
#include <QtGui/QPainter>
#include "../../../qrutils/graphicsUtils/rectangleImpl.h"

class Ellipse : public Item
{
public:
	Ellipse(qreal x1, qreal y1, qreal x2, qreal y2, Item* parent = 0);
	Ellipse(Ellipse const &other);
	virtual Item* clone();
	virtual QRectF boundingRect() const;
	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);

	virtual QPair<QDomElement, Item::DomElementTypes> generateItem(QDomDocument &document, QPoint const &topLeftPicture);

private:
	graphicsUtils::RectangleImpl mRectangleImpl;
};
