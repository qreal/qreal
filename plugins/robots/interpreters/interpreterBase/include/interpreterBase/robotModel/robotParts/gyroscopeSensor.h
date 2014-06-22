#pragma once

#include "scalarSensor.h"
#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

/// Base class for gyroscope sensors.
class ROBOTS_INTERPRETER_BASE_EXPORT GyroscopeSensor : public ScalarSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "gyroscope")
	Q_CLASSINFO("friendlyName", tr("Gyroscope"))

public:
	/// Constructor, takes device type info and port on which this sensor is configured.
	GyroscopeSensor(DeviceInfo const &info, PortInfo const &port);
};

}
}
}
