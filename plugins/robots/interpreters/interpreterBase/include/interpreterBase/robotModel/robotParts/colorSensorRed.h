#pragma once

#include "colorSensor.h"

#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

/// Base class for sensor that returns intensity of red color under it.
class ROBOTS_INTERPRETER_BASE_EXPORT ColorSensorRed : public ColorSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "colorRed")
	Q_CLASSINFO("friendlyName", tr("Color sensor (red)"))

public:
	/// Constructor, takes device type info and port on which this sensor is configured.
	ColorSensorRed(DeviceInfo const &info, PortInfo const &port);
};

}
}
}
