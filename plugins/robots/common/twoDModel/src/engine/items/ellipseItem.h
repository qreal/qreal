#pragma once

#include <qrutils/graphicsUtils/rectangleImpl.h>

#include "colorFieldItem.h"

namespace twoDModel {
namespace items {

class EllipseItem : public ColorFieldItem
{
	Q_OBJECT

public:
	EllipseItem(const QPointF &begin, const QPointF &end);

	AbstractItem* clone() const override;

	QRectF boundingRect() const override;
	QRectF calcNecessaryBoundingRect() const override;
	void drawItem(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

	QDomElement serialize(QDomDocument &document, const QPoint &topLeftPicture) override;
	void deserialize(const QDomElement &element) override;

	QPainterPath shape() const override;

private:
	void setPrivateData();

	graphicsUtils::RectangleImpl mEllipseImpl;
};

}
}
