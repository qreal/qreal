#pragma once

#include <qrutils/graphicsUtils/abstractItem.h>

namespace twoDModel {
namespace items {

class ColorFieldItem : public graphicsUtils::AbstractItem
{
public:
	explicit ColorFieldItem(QGraphicsItem* parent = 0);
	~ColorFieldItem() override;

	void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0) override = 0;

	virtual QDomElement serialize(QDomDocument &document, const QPoint &topLeftPicture) = 0;
};

}
}
