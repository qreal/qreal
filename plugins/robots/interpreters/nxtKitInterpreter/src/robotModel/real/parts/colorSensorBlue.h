#pragma once

#include "colorSensor.h"

namespace nxtKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class ColorSensorBlue : public ColorSensor
{
public:
	static QString friendlyName();

	ColorSensorBlue(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, utils::robotCommunication::RobotCommunicator &robotCommunicator);


};

}
}
}
}
