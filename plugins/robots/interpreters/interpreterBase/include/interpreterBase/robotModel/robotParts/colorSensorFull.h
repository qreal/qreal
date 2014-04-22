#pragma once

#include "colorSensor.h"

#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

class ROBOTS_INTERPRETER_BASE_EXPORT ColorSensorFull : public ColorSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "colorRecognition")
	Q_CLASSINFO("friendlyName", tr("Color sensor (full)"))

public:
	ColorSensorFull(DeviceInfo const &info, PortInfo const &port);
};

}
}
}
