#pragma once
#include "unrealSensorImplementation.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace sensorImplementations {

class UnrealColorSensorImplementation : public UnrealSensorImplementation
{
	Q_OBJECT
public:
	UnrealColorSensorImplementation(
			robots::enums::inputPort::InputPortEnum const port
			, d2Model::D2RobotModel *d2Model
			, robots::enums::sensorType::SensorTypeEnum const &sensorType
			);

	virtual void read();
};

}
}
}
}
}
}
