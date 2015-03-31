#pragma once

#include "colorSensor.h"

#include "kitBase/kitBaseDeclSpec.h"

namespace kitBase {
namespace robotModel {
namespace robotParts {

/// Base class for sensor that returns intensity of blue color under it.
class ROBOTS_KIT_BASE_EXPORT ColorSensorBlue : public ColorSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "colorBlue")
	Q_CLASSINFO("friendlyName", tr("Color sensor (blue)"))

public:
	/// Constructor, takes device type info and port on which this sensor is configured.
	ColorSensorBlue(const DeviceInfo &info, const PortInfo &port);
};

}
}
}
