#pragma once

#include <interpreterBase/robotModel/robotParts/button.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace parts {

/// Pad of android gamepad as a button that knows if it is pressed or not.
class TrikGamepadPadPressSensor : public interpreterBase::robotModel::robotParts::Button
{
	Q_OBJECT
	Q_CLASSINFO("name", "gamepadPadPressSensor")
	Q_CLASSINFO("friendlyName", tr("Android Gamepad Pad as Button"))

public:
	/// Constructor.
	/// @param info - device info for that device.
	/// @param port - port on which this device is configured.
	TrikGamepadPadPressSensor(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port);
};

}
}
}
