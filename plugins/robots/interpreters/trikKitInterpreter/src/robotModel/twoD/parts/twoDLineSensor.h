#pragma once

#include "robotModel/parts/trikLineSensor.h"

namespace trikKitInterpreter {
namespace robotModel {
namespace twoD {
namespace parts {

/// Stub for TRIK color sensor for interpreter.
/// Does nothing until line sensor emulation will be requested for 2D model.
class LineSensor : public robotModel::parts::TrikLineSensor
{
	Q_OBJECT

public:
	LineSensor(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port);

	void init() override;
	void detectLine() override;
	void read() override;
};

}
}
}
}
