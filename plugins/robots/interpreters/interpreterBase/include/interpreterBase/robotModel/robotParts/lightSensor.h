#pragma once

#include "scalarSensor.h"
#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

/// Base class for light sensors.
class ROBOTS_INTERPRETER_BASE_EXPORT LightSensor : public ScalarSensor
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
