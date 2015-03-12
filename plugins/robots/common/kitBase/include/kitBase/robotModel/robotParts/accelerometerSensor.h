#pragma once

#include "scalarSensor.h"
#include "kitBase/kitBaseDeclSpec.h"

namespace kitBase {
namespace robotModel {
namespace robotParts {

/// Abstract accelerometer sensor.
/// @todo: Make it vector
class ROBOTS_KIT_BASE_EXPORT AccelerometerSensor : public ScalarSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "accelerometer")
	Q_CLASSINFO("friendlyName", tr("Accelerometer"))

public:
	/// Constructor, takes device type info and port on which this sensor is configured.
	AccelerometerSensor(const DeviceInfo &info, const PortInfo &port);
};

}
}
}
