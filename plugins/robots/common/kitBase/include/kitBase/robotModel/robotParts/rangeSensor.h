#pragma once

#include "scalarSensor.h"

#include "kitBase/kitBaseDeclSpec.h"

namespace kitBase {
namespace robotModel {
namespace robotParts {

/// Base class for sensors that return a distance, for example sonar or IR range sensor.
class ROBOTS_KIT_BASE_EXPORT RangeSensor : public ScalarSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "sonar")
	Q_CLASSINFO("friendlyName", tr("Range sensor"))

public:
	/// Constructor, takes device type info and port on which this sensor is configured.
	RangeSensor(const DeviceInfo &info, const PortInfo &port);
};

}
}
}
