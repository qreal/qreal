#pragma once

#include <interpreterBase/robotModel/robotParts/touchSensor.h>
#include <utils/robotCommunication/robotCommunicator.h>

#include "commandConstants.h"
#include "ev3InputDevice.h"

namespace ev3KitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class TouchSensor : public interpreterBase::robotModel::robotParts::TouchSensor
{
	Q_OBJECT

public:
	TouchSensor(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port
			, utils::robotCommunication::RobotCommunicator &robotCommunicator);

	void read() override;

private:
	Ev3InputDevice mImplementation;
	utils::robotCommunication::RobotCommunicator &mRobotCommunicator;
};

}
}
}
}
