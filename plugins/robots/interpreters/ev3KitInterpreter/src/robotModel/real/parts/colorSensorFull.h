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
	ColorSensorFull(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
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
