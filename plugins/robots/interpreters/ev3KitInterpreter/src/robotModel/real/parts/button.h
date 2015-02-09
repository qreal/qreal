#pragma once

#include <interpreterBase/robotModel/robotParts/button.h>
#include <utils/robotCommunication/robotCommunicator.h>

#include "commandConstants.h"

namespace ev3KitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class Button : public interpreterBase::robotModel::robotParts::Button
{
	Q_OBJECT

public:
	Button(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, utils::robotCommunication::RobotCommunicator &robotCommunicator);

	void read() override;
private:
	char parsePort(const QString &portName);

	utils::robotCommunication::RobotCommunicator &mRobotCommunicator;
};

}
}
}
}
