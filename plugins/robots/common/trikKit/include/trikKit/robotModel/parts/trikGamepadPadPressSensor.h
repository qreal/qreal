#pragma once

#include <kitBase/robotModel/robotParts/button.h>

namespace trik {
namespace robotModel {
namespace parts {

/// Pad of android gamepad as a button that knows if it is pressed or not.
class TrikGamepadPadPressSensor : public kitBase::robotModel::robotParts::Button
{
	Q_OBJECT
	Q_CLASSINFO("name", "gamepadPadPressSensor")
	Q_CLASSINFO("friendlyName", tr("Android Gamepad Pad as Button"))

public:
	/// Constructor.
	/// @param info - device info for that device.
	/// @param port - port on which this device is configured.
	TrikGamepadPadPressSensor(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port);
};

}
}
}
