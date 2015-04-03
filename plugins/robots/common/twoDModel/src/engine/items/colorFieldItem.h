#pragma once

#include <qrutils/graphicsUtils/abstractItem.h>

namespace twoDModel {
namespace items {

class ColorFieldItem : public graphicsUtils::AbstractItem
{
public:
	explicit ColorFieldItem(QGraphicsItem* parent = 0);
	~ColorFieldItem() override;

	/// Creates a copy of this graphical item. Transfers ownership to the caller.
	virtual AbstractItem *clone() const = 0;

	virtual QRectF boundingRect() const = 0;
	void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0) override = 0;

	virtual QDomElement serialize(QDomDocument &document, const QPoint &topLeftPicture) = 0;
};

}
}
