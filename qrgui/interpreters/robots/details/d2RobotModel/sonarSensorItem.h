#pragma once

#include "sensorItem.h"
#include "worldModel.h"
#include "sensorsConfiguration.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

/** @brief Class that represents sonar sensor that can draw its scanning area */
class SonarSensorItem : public SensorItem
{
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)

public:
	SonarSensorItem(WorldModel const &worldModel, SensorsConfiguration &configuration, inputPort::InputPortEnum port);
	void paint(QPainter *painter, QStyleOptionGraphicsItem const *style, QWidget *widget);
	QRectF boundingRect() const;

private:
	WorldModel const &mWorldModel;

	/// Returns scanning region of a sensor as painter path, in relative to sensor coordinates
	QPainterPath scanningRegion() const;
};

}
}
}
}
}
