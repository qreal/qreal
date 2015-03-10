#pragma once

#include "scalarSensor.h"

#include "kitBase/kitBaseDeclSpec.h"

namespace kitBase {
namespace robotModel {
namespace robotParts {

/// Base class for touch sensor, which can return how hard it is being pushed.
class ROBOTS_KIT_BASE_EXPORT TouchSensor : public ScalarSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "touch")
	Q_CLASSINFO("friendlyName", tr("Touch sensor"))

public:
	/// Constructor, takes device type info and port on which this sensor is configured.
	TouchSensor(const DeviceInfo &info, const PortInfo &port);
};

}
}
}
