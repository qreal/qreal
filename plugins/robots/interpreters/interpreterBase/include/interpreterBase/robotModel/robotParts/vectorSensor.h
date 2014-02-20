#pragma once

#include "abstractSensor.h"

#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

class ROBOTS_INTERPRETER_BASE_EXPORT VectorSensor : public AbstractSensor
{
	Q_OBJECT

public:
	VectorSensor(DeviceInfo const &info, PortInfo const &port);
};

}
}
}
