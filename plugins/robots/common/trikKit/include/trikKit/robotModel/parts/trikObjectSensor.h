#pragma once

#include <kitBase/robotModel/robotParts/vectorSensor.h>

namespace trik {
namespace robotModel {
namespace parts {

/// Device representing TRIK camera object detector.
class TrikObjectSensor : public kitBase::robotModel::robotParts::VectorSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "trikObjectSensor")
	Q_CLASSINFO("friendlyName", tr("Object Sensor"))

public:
	TrikObjectSensor(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port);

	/// Turns camera on and prepares a sensor.
	virtual void init() = 0;

	/// Locks dominant color at center of a field of view of a camera as object color and begins tracking it.
	virtual void detect() = 0;
};

}
}
}
