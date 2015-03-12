#pragma once

#include <kitBase/robotModel/robotParts/vectorSensor.h>

namespace trik {
namespace robotModel {
namespace parts {

/// Pad of android gamepad as 2d-sensor reporting current finger coordinates from -100 to 100
/// (-100, -100) is bottom left corner of a pad. (-1, -1) is returned if pad is not pressed.
class TrikGamepadPad : public kitBase::robotModel::robotParts::VectorSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "gamepadPad")
	Q_CLASSINFO("friendlyName", tr("Android Gamepad Pad"))

public:
	/// Constructor.
	/// @param info - device info for that device.
	/// @param port - port on which this device is configured.
	TrikGamepadPad(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port);
};

}
}
}
