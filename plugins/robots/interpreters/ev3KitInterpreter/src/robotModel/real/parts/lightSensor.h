#pragma once

#include <kitBase/robotModel/robotParts/lightSensor.h>
#include <utils/robotCommunication/robotCommunicator.h>

#include "ev3InputDevice.h"

namespace ev3 {
namespace robotModel {
namespace real {
namespace parts {

class LightSensor : public kitBase::robotModel::robotParts::LightSensor
{
	Q_OBJECT

public:
	LightSensor(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
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
