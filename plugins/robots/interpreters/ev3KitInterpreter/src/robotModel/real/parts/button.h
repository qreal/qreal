#pragma once

#include <kitBase/robotModel/robotParts/button.h>
#include <utils/robotCommunication/robotCommunicator.h>

namespace ev3 {
namespace robotModel {
namespace real {
namespace parts {

class Button : public kitBase::robotModel::robotParts::Button
{
	Q_OBJECT

public:
	Button(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
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
