#pragma once

#include "robotModel/parts/trikColorSensor.h"
#include <utils/tcpRobotCommunicator.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

/// Stub for TRIK color sensor for interpreter.
class ColorSensor : public robotModel::parts::TrikColorSensor
{
	Q_OBJECT

public:
	ColorSensor(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port
			, utils::TcpRobotCommunicator &robotCommunicator);

	void init() override;
	void read() override;

private:
	utils::TcpRobotCommunicator &mRobotCommunicator;
};

}
}
}
}
