#pragma once

#include "scalarSensor.h"
#include "kitBase/kitBaseDeclSpec.h"

namespace kitBase {
namespace robotModel {
namespace robotParts {

/// Base class for light sensors.
class ROBOTS_KIT_BASE_EXPORT LightSensor : public ScalarSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "light")
	Q_CLASSINFO("friendlyName", tr("Light sensor"))

public:
	/// Constructor, takes device type info and port on which this sensor is configured.
	LightSensor(const DeviceInfo &info, const PortInfo &port);
};

}
}
}
