#pragma once

#include <QtGui/QGraphicsItem>
#include <QtGui/QPainter>

#include "sensorItem.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

const qreal robotWidth = 50;
const qreal robotHeight = 50;

/** @brief Class that represents a robot in 2D model */
class RobotItem : public QObject, public QGraphicsItem
{
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)
public:
	RobotItem();

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *style, QWidget *widget);
	QRectF boundingRect() const;
	void mousePressEvent(QGraphicsSceneMouseEvent * event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
	void setPos(QPointF const &newPos);

	/** @brief Add new sensor to mSensors */
	void addSensor(SensorItem *sensor);
	void removeSensor(SensorItem *sensor);
	/** @brief Clear mSensors */
	void clearSensors();

	/** @brief Returns false if we're dragging robot item somewhere */
	bool isOnTheGround();

signals:
	void changedPosition();

private:
	/** @brief Image of a robot drawn on scene */
	QImage mImage;

	/** @brief List of sensors added to robot */
	QList<SensorItem *> mSensors;
	/** @brief Previous position of robot (used while dragging to update sensors positions)*/
	QPointF mPreviousPos;
	bool mIsOnTheGround;
};

}
}
}
}
}
