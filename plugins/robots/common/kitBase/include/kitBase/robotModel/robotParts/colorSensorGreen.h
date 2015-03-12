#pragma once

#include "colorSensor.h"

#include "kitBase/kitBaseDeclSpec.h"

namespace kitBase {
namespace robotModel {
namespace robotParts {

/// Base class for sensor that returns intensity of green color under it.
class ROBOTS_KIT_BASE_EXPORT ColorSensorGreen : public ColorSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "colorGreen")
	Q_CLASSINFO("friendlyName", tr("Color sensor (green)"))

public:
	/// Constructor, takes device type info and port on which this sensor is configured.
	ColorSensorGreen(const DeviceInfo &info, const PortInfo &port);
};

}
}
}
