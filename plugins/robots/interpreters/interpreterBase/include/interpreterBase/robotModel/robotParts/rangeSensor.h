#pragma once

#include "scalarSensor.h"

#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

/// Base class for sensors that return a distance, for example sonar or IR range sensor.
class ROBOTS_INTERPRETER_BASE_EXPORT RangeSensor : public ScalarSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "sonar")
	Q_CLASSINFO("friendlyName", tr("Range sensor"))

public:
	/// Constructor, takes device type info and port on which this sensor is configured.
	RangeSensor(DeviceInfo const &info, PortInfo const &port);
};

}
}
}
