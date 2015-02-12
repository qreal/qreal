#pragma once

#include <interpreterBase/robotModel/robotParts/vectorSensor.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace parts {

/// Device representing TRIK camera line detector.
class TrikLineSensor : public interpreterBase::robotModel::robotParts::VectorSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "trikLineSensor")
	Q_CLASSINFO("friendlyName", tr("Line Sensor"))

public:
	TrikLineSensor(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port);

	/// Turns camera on and prepares a sensor.
	virtual void init() = 0;

	/// Locks dominant color at center of a field of view of a camera as line color and begins tracking it.
	virtual void detectLine() = 0;
};

}
}
}
