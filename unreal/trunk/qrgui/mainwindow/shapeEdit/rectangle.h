#pragma once
#include "item.h"
#include <QtGui/QPainter>

class Rectangle : public Item
{
public:
	Rectangle(qreal x1, qreal y1, qreal x2, qreal y2, Item* parent = 0);
	virtual QRectF boundingRect() const;
	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);

	virtual QPair<QDomElement, Item::DomElementTypes> generateItem(QDomDocument &document, QPoint const &topLeftPicture);
};
