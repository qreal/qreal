#pragma once

#include "scalarSensor.h"

#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

class ROBOTS_INTERPRETER_BASE_EXPORT TouchSensor : public ScalarSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "touch")
	Q_CLASSINFO("friendlyName", tr("Touch sensor"))

public:
	TouchSensor(DeviceInfo const &info, PortInfo const &port);
};

}
}
}
