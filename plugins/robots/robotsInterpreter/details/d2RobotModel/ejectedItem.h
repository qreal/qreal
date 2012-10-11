#pragma once
#include <QtGui/QGraphicsItem>
#include "colorFieldItem.h"
#include "../../../../../qrutils/graphicsUtils/rectangleImpl.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

class EjectedItem : public QObject, public ColorFieldItem
{
	Q_OBJECT
public:
	EjectedItem(QPointF begin, QPointF end);
	virtual QRectF calcNecessaryBoundingRect() const;
	virtual QRectF boundingRect() const;
	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);

	virtual QDomElement serialize(QDomDocument &document, QPoint const &topLeftPicture);
	virtual void deserialize(QDomElement const &element);

public slots:
	void robotChangedPosition(QPointF const &newPos);

protected:
	graphicsUtils::RectangleImpl mRectImpl;
	void setPrivateData();

private:
	QString mSerializeName;
};
}
}
}
}
}
