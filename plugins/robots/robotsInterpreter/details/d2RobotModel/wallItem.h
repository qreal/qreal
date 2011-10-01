#pragma once

#include <QtGui/QGraphicsItem>
#include <QtGui/QPainter>
#include <QtXml/QDomDocument>
#include "lineItem.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

class WallItem : public LineItem
{
	Q_OBJECT

public:
	WallItem(QPointF const &begin, QPointF const &end);
	QPointF begin();
	QPointF end();

	/// Draws selection rect around sensorBoundingBox
	virtual void drawExtractionForItem(QPainter *painter);
	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

	virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

protected:
	virtual void setPrivateData();
private:
	bool mDragged;
	QImage mImage;
};

}
}
}
}
}

