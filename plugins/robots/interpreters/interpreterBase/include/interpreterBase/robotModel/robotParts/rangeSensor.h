#pragma once

#include "scalarSensor.h"

#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

class ROBOTS_INTERPRETER_BASE_EXPORT RangeSensor : public ScalarSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "sonar")
	Q_CLASSINFO("friendlyName", tr("Range sensor"))

public:
	RangeSensor(DeviceInfo const &info, PortInfo const &port);
};

}
}
}
