#pragma once

#include "colorSensor.h"

#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

/// Base class for sensor that returns intensity of green color under it.
class ROBOTS_INTERPRETER_BASE_EXPORT ColorSensorGreen : public ColorSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "colorGreen")
	Q_CLASSINFO("friendlyName", tr("Color sensor (green)"))

public:
	/// Constructor, takes device type info and port on which this sensor is configured.
	ColorSensorGreen(DeviceInfo const &info, PortInfo const &port);
};

}
}
}
