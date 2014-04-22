#pragma once

#include "vectorSensor.h"
#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

class ROBOTS_INTERPRETER_BASE_EXPORT AccelerometerSensor : public VectorSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "accelerometer")
	Q_CLASSINFO("friendlyName", tr("Accelerometer"))

public:
	AccelerometerSensor(DeviceInfo const &info, PortInfo const &port);
};

}
}
}
