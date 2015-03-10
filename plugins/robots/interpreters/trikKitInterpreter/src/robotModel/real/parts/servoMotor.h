#pragma once

#include <trikKit/robotModel/parts/trikServoMotor.h>
#include <utils/tcpRobotCommunicator.h>

namespace trik {
namespace robotModel {
namespace real {
namespace parts {

class ServoMotor : public robotModel::parts::TrikServoMotor
{
	Q_OBJECT

public:
	ServoMotor(const kitBase::robotModel::DeviceInfo &info
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
