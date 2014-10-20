#pragma once

#include "robotModel/parts/trikColorSensor.h"

namespace trikKitInterpreter {
namespace robotModel {
namespace twoD {
namespace parts {

/// Stub for TRIK color sensor for interpreter.
/// Does nothing until color sensor emulation will be requested for 2D model.
class ColorSensor : public robotModel::parts::TrikColorSensor
{
	Q_OBJECT

public:
	ColorSensor(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port);

	void init() override;
	void read() override;
};

}
}
}
}
