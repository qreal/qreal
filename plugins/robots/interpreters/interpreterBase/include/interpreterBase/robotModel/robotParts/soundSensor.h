#pragma once

#include "scalarSensor.h"

#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

/// Sensor that measures loudness.
class ROBOTS_INTERPRETER_BASE_EXPORT SoundSensor : public ScalarSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "sound")
	Q_CLASSINFO("friendlyName", tr("Sound sensor"))

public:
	/// Constructor, takes device type info and port on which this sensor is configured.
	SoundSensor(const DeviceInfo &info, const PortInfo &port);
};

}
}
}
