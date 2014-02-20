#pragma once

#include "scalarSensor.h"
#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

class ROBOTS_INTERPRETER_BASE_EXPORT EncoderSensor : public ScalarSensor
{
public:
	static QString friendlyName();
	static DeviceInfo::Direction direction();

	EncoderSensor(DeviceInfo const &info, PortInfo const &port);
};

}
}
}
