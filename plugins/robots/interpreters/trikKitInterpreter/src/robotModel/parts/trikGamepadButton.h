#pragma once

#include <interpreterBase/robotModel/robotParts/button.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace parts {

/// "Magic button" of android gamepad.
class TrikGamepadButton : public interpreterBase::robotModel::robotParts::Button
{
	Q_OBJECT
	Q_CLASSINFO("name", "gamepadButton")
	Q_CLASSINFO("friendlyName", tr("Android Gamepad Button"))

public:
	/// Constructor.
	/// @param info - device info for that device.
	/// @param port - port on which this device is configured.
	TrikGamepadButton(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port);
};

}
}
}
