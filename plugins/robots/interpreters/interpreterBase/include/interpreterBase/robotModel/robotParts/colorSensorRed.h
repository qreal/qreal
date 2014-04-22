#pragma once

#include "colorSensor.h"

#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

class ROBOTS_INTERPRETER_BASE_EXPORT ColorSensorRed : public ColorSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "colorRed")
	Q_CLASSINFO("friendlyName", tr("Color sensor (red)"))

public:
	ColorSensorRed(DeviceInfo const &info, PortInfo const &port);
};

}
}
}
