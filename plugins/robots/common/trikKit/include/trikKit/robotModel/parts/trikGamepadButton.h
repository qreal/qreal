#pragma once

#include <kitBase/robotModel/robotParts/button.h>

namespace trik {
namespace robotModel {
namespace parts {

/// "Magic button" of android gamepad.
class TrikGamepadButton : public kitBase::robotModel::robotParts::Button
{
	Q_OBJECT
	Q_CLASSINFO("name", "gamepadButton")
	Q_CLASSINFO("friendlyName", tr("Android Gamepad Button"))

public:
	/// Constructor.
	/// @param info - device info for that device.
	/// @param port - port on which this device is configured.
	TrikGamepadButton(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port);
};

}
}
}
