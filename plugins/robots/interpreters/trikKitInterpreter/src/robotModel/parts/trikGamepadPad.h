#pragma once

#include <interpreterBase/robotModel/robotParts/vectorSensor.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace parts {

/// Pad of android gamepad as 2d-sensor reporting current finger coordinates from -100 to 100
/// (-100, -100) is bottom left corner of a pad. (-1, -1) is returned if pad is not pressed.
class TrikGamepadPad : public interpreterBase::robotModel::robotParts::VectorSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "gamepadPad")
	Q_CLASSINFO("friendlyName", tr("Android Gamepad Pad"))

public:
	/// Constructor.
	/// @param info - device info for that device.
	/// @param port - port on which this device is configured.
	TrikGamepadPad(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port);
};

}
}
}
