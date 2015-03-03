#pragma once

#include "scalarSensor.h"
#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

/// Encoder on motor, returns current count of rotations of motor axis after the beginning of measurement.
class ROBOTS_INTERPRETER_BASE_EXPORT EncoderSensor : public ScalarSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "encoder")
	Q_CLASSINFO("friendlyName", tr("Encoder"))

public:
	/// Constructor, takes device type info and port on which this sensor is configured.
	EncoderSensor(const DeviceInfo &info, const PortInfo &port);

	/// Clears encoder readings, and makes this sensor return 0, like a robot is turned on just now.
	/// @todo This name is strange.
	virtual void nullify() = 0;
};

}
}
}
