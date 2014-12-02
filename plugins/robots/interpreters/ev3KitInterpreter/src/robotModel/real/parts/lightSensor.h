#pragma once

#include <interpreterBase/robotModel/robotParts/lightSensor.h>
#include <utils/robotCommunication/robotCommunicator.h>

#include "commandConstants.h"
#include "ev3InputDevice.h"

namespace ev3KitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class LightSensor : public interpreterBase::robotModel::robotParts::LightSensor
{
	Q_OBJECT

public:
	LightSensor(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
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
