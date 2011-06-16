#pragma once

#include "sensorsConfiguration.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

class RobotModelInterface
{
public:
	virtual SensorsConfiguration &configuration() = 0;

	virtual void addSensor(inputPort::InputPortEnum const port, sensorType::SensorTypeEnum const type
					, QPoint const &position, qreal const &direction) = 0;

};

}
}
}
}
}
