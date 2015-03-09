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
	PowerMotor(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port
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
