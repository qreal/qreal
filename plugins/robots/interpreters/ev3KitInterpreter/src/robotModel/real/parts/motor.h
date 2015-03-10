#pragma once

#include <kitBase/robotModel/portInfo.h>
#include <utils/robotCommunication/robotCommunicator.h>
#include <ev3Kit/robotModel/parts/ev3Motor.h>

namespace ev3 {
namespace robotModel {
namespace real {
namespace parts {

class Motor : public ev3::robotModel::parts::Ev3Motor
{
	Q_OBJECT

public:
	Motor(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, utils::robotCommunication::RobotCommunicator &robotCommunicator);

	void on(int speed) override;
	void stop() override;
	void off() override;

private:
	char parsePort(QChar portName);

	utils::robotCommunication::RobotCommunicator &mRobotCommunicator;
};

}
}
}
}
