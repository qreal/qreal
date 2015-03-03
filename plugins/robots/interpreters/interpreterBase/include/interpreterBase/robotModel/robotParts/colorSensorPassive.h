#pragma once

#include "colorSensor.h"

#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

/// Base class for sensor that returns intensity of white under it, so it works as generic light sensor. It is needed
/// because NXT uses it as one of the modes of color sensor.
/// @todo Rewrite 2D model and this stuff to avoid NXT dependencies. We shall have our own means to simulate complex
///       sensors like NXT color.
class ROBOTS_INTERPRETER_BASE_EXPORT ColorSensorPassive : public ColorSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "colorNone")
	Q_CLASSINFO("friendlyName", tr("Color sensor (passive)"))

public:
	/// Constructor, takes device type info and port on which this sensor is configured.
	ColorSensorPassive(const DeviceInfo &info, const PortInfo &port);
};

}
}
}
