#pragma once

#include "colorSensor.h"

namespace nxtKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class ColorSensorFull : public ColorSensor
{
	Q_OBJECT
	Q_CLASSINFO("friendlyName", tr("Color sensor (full)"))

public:
	ColorSensorFull(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, utils::robotCommunication::RobotCommunicator &robotCommunicator);
};

}
}
}
}
