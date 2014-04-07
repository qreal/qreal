#pragma once

#include "scalarSensor.h"

#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

class ROBOTS_INTERPRETER_BASE_EXPORT SoundSensor : public ScalarSensor
{
	Q_OBJECT
	Q_CLASSINFO("friendlyName", tr("Sound sensor"))

public:
	SoundSensor(DeviceInfo const &info, PortInfo const &port);
};

}
}
}
