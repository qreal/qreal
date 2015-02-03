#pragma once

#include <interpreterBase/robotModel/robotParts/scalarSensor.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace parts {

/// Gamepad connection indicator, shows whether Android gamepad is connected to robot or not.
class TrikGamepadConnectionIndicator : public interpreterBase::robotModel::robotParts::ScalarSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "gamepadConnectionIndicator")
	Q_CLASSINFO("friendlyName", tr("Android Gamepad Connection Indicator"))

public:
	/// Constructor.
	/// @param info - device info for that device.
	/// @param port - port on which this device is configured.
	TrikGamepadConnectionIndicator(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port);
};

}
}
}
