#pragma once

#include <trikKit/robotModel/parts/trikPowerMotor.h>
#include <utils/tcpRobotCommunicator.h>

namespace trik {
namespace robotModel {
namespace real {
namespace parts {

class PowerMotor : public robotModel::parts::TrikPowerMotor
{
	Q_OBJECT

public:
	PowerMotor(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
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
