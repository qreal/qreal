#pragma once
#include <QtCore/QList>
#include <QtWidgets/QGraphicsItem>
#include <QtGui/QPainter>
#include <QtXml/QDomDocument>
#include "colorFieldItem.h"
#include "../../../../../qrutils/graphicsUtils/abstractItem.h"
#include "../../../../../qrutils/graphicsUtils/rectangleImpl.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

class EllipseItem : public QObject, public ColorFieldItem
{
	Q_OBJECT
public:
	EllipseItem(QPointF begin, QPointF end);

	virtual QRectF boundingRect() const;
	virtual QRectF calcNecessaryBoundingRect() const;
	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	virtual QDomElement serialize(QDomDocument &document, QPoint const &topLeftPicture);
	virtual void deserialize(QDomElement const &element);

private:
	graphicsUtils::RectangleImpl mEllipseImpl;
	void setPrivateData();
};
}
}
}
}
}
