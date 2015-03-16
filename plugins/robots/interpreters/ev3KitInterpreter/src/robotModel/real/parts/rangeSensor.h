#pragma once

#include <kitBase/robotModel/robotParts/rangeSensor.h>
#include <utils/robotCommunication/robotCommunicator.h>

#include "ev3InputDevice.h"

namespace ev3 {
namespace robotModel {
namespace real {
namespace parts {

class RangeSensor : public kitBase::robotModel::robotParts::RangeSensor
{
	Q_OBJECT

public:
	RangeSensor(const kitBase::robotModel::DeviceInfo &info
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
