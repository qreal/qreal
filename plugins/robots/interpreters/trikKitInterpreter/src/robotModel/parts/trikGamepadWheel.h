#pragma once

#include <interpreterBase/robotModel/robotParts/scalarSensor.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace parts {

/// Sensor that reports current tilt angle of Android gamepad, if "wheel" is turned on. Max left tilt is -100,
/// max right tilt is reported as 100.
class TrikGamepadWheel: public interpreterBase::robotModel::robotParts::ScalarSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "gamepadWheel")
	Q_CLASSINFO("friendlyName", tr("Android Gamepad Wheel"))

public:
	/// Constructor.
	/// @param info - device info for that device.
	/// @param port - port on which this device is configured.
	TrikGamepadWheel(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port);
};

}
}
}
