#pragma once

#include "robotModel/parts/trikPowerMotor.h"
#include <utils/tcpRobotCommunicator.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class PowerMotor : public robotModel::parts::TrikPowerMotor
{
	Q_OBJECT

public:
	PowerMotor(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, utils::TcpRobotCommunicator &tcpRobotCommunicator);

	void on(int speed) override;
	void stop() override;
	void off() override;

private:
	utils::TcpRobotCommunicator &mRobotCommunicator;
};

}
}
}
}
