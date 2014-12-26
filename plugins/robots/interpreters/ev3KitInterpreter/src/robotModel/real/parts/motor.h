#pragma once

#include <interpreterBase/robotModel/portInfo.h>
#include <utils/robotCommunication/robotCommunicator.h>
#include "src/robotModel/parts/ev3Motor.h"

#include "commandConstants.h"

namespace ev3KitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class Motor : public ev3KitInterpreter::robotModel::parts::Ev3Motor
{
	Q_OBJECT

public:
	Motor(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
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
