#pragma once

#include "scalarSensor.h"
#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

class ROBOTS_INTERPRETER_BASE_EXPORT GyroscopeSensor : public ScalarSensor
{
	Q_OBJECT
	Q_CLASSINFO("friendlyName", tr("Gyroscope"))

public:
	GyroscopeSensor(DeviceInfo const &info, PortInfo const &port);
};

}
}
}
