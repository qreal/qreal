#pragma once

#include "colorSensor.h"

#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

/// Base class for sensor that returns intensity of blue color under it.
class ROBOTS_INTERPRETER_BASE_EXPORT ColorSensorBlue : public ColorSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "colorBlue")
	Q_CLASSINFO("friendlyName", tr("Color sensor (blue)"))

public:
	/// Constructor, takes device type info and port on which this sensor is configured.
	ColorSensorBlue(DeviceInfo const &info, PortInfo const &port);
};

}
}
}
