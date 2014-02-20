#pragma once

#include "scalarSensor.h"

#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

class ROBOTS_INTERPRETER_BASE_EXPORT TouchSensor : public ScalarSensor
{
	Q_OBJECT

public:
	static QString friendlyName();
	static DeviceInfo::Direction direction();

	TouchSensor(DeviceInfo const &info, PortInfo const &port);
};

}
}
}
