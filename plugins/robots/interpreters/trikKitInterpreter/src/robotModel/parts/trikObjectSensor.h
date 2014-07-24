#pragma once

#include <interpreterBase/robotModel/robotParts/vectorSensor.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace parts {

/// Device representing TRIK camera object detector.
class TrikObjectSensor : public interpreterBase::robotModel::robotParts::VectorSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "trikObjectSensor")
	Q_CLASSINFO("friendlyName", tr("Object Sensor"))

public:
	TrikObjectSensor(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port);

	/// Turns camera on and prepares a sensor.
	virtual void init() = 0;

	/// Locks dominant color at center of a field of view of a camera as object color and begins tracking it.
	virtual void detect() = 0;
};

}
}
}
