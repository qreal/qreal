#pragma once
#include <QtXml/QDomDocument>
#include <QtGui/QGraphicsItem>
#include "../../../../../qrutils/graphicsUtils/abstractItem.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

class ColorFieldItem : public graphicsUtils::AbstractItem
{
public:
	ColorFieldItem(QGraphicsItem* parent = 0);

	virtual QRectF boundingRect() const = 0;
	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0) = 0;

	virtual QDomElement serialize(QDomDocument &document) = 0;
	virtual void deserialize(QDomElement const &element) = 0;
};

}
}
}
}
}
