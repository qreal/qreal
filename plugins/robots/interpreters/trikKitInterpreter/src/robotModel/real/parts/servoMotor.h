#pragma once

#include "robotModel/parts/trikServoMotor.h"
#include <utils/tcpRobotCommunicator.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class ServoMotor : public robotModel::parts::TrikServoMotor
{
	Q_OBJECT

public:
	ServoMotor(const interpreterBase::robotModel::DeviceInfo &info
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
