#pragma once

#include <interpreterBase/robotModel/robotParts/colorSensorFull.h>
#include <utils/robotCommunication/robotCommunicator.h>

#include "implementations/colorSensorImpl.h"

namespace ev3KitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class ColorSensorFull : public interpreterBase::robotModel::robotParts::ColorSensorFull
{
	Q_OBJECT

public:
	ColorSensorFull(const interpreterBase::robotModel::DeviceInfo &info
			  , const interpreterBase::robotModel::PortInfo &port
			  , utils::robotCommunication::RobotCommunicator &robotCommunicator);

	void read() override;

private:
	ColorSensorImpl mColorSensor;
	utils::robotCommunication::RobotCommunicator &mRobotCommunicator;
};

}
}
}
}
