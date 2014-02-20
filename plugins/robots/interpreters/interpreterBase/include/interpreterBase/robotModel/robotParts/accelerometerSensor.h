#pragma once

#include "vectorSensor.h"
#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

class ROBOTS_INTERPRETER_BASE_EXPORT AccelerometerSensor : public VectorSensor
{
	Q_OBJECT

public:
	static QString friendlyName();
	static DeviceInfo::Direction direction();

	AccelerometerSensor(DeviceInfo const &info, PortInfo const &port);
};

}
}
}
