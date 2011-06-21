#pragma once

#include <QtGui/QGraphicsItem>
#include <QtGui/QPainter>

#include "sensorsConfiguration.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

/** @brief Class that represents robot port in 2D model */
class SensorItem : public QObject, public QGraphicsItem
{
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)

public:
	SensorItem(SensorsConfiguration &configuration, inputPort::InputPortEnum port);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *style, QWidget *widget);

	/// Draws selection rect around sensorBoundingBox
	void drawSelection(QPainter *painter, QRectF sensorBoundingBox);

	QRectF boundingRect() const;

	void mousePressEvent(QGraphicsSceneMouseEvent * event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

	void setBasePosition(QPointF const &pos);

protected:
	static int const size = 6;

	SensorsConfiguration &mConfiguration;
	inputPort::InputPortEnum const mPort;
	QPointF mBasePos;
	bool mDragged;

	QColor color() const;
};

}
}
}
}
}
