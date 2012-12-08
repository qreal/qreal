#pragma once

#include <QtGui/QGraphicsItem>
#include <QtGui/QPainter>
#include <QtXml/QDomDocument>
#include "../../../../../qrutils/graphicsUtils/lineImpl.h"
#include "colorFieldItem.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

class LineItem : public QObject, public ColorFieldItem
{
	Q_OBJECT

public:
	LineItem(QPointF begin, QPointF end);
	QPainterPath shape() const;
	virtual QRectF boundingRect() const;
	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	virtual void drawExtractionForItem(QPainter* painter);

	virtual void resizeItem(QGraphicsSceneMouseEvent *event);
	virtual void reshapeRectWithShift();


        virtual QDomElement serialize(QDomDocument &document, QPoint const &topLeftPicture);
	virtual void deserialize(QDomElement const &element);
        void setSerializeName(QString name);

protected:
	void setPrivateData();
	graphicsUtils::LineImpl mLineImpl;

private:
	QString mSerializeName;
};
}
}
}
}
}
