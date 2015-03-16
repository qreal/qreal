#pragma once

#include "scalarSensor.h"

#include "kitBase/kitBaseDeclSpec.h"

namespace kitBase {
namespace robotModel {
namespace robotParts {

/// Sensor that measures loudness.
class ROBOTS_KIT_BASE_EXPORT SoundSensor : public ScalarSensor
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
