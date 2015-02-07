#pragma once

#include <QtGui/QPainter>

#include <qrutils/graphicsUtils/rectangleImpl.h>

#include "mainWindow/shapeEdit/item.h"

class QRealEllipse : public Item
{
public:
	QRealEllipse(qreal x1, qreal y1, qreal x2, qreal y2, Item* parent = 0);
	QRealEllipse(const QRealEllipse &other);
	virtual Item* clone();
	virtual QRectF boundingRect() const;
	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);

	virtual QPair<QDomElement, Item::DomElementTypes> generateItem(QDomDocument &document, const QPoint &topLeftPicture);

private:
	graphicsUtils::RectangleImpl mRectangleImpl;
};
