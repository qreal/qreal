#pragma once

#include <interpreterBase/robotModel/robotParts/motor.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace parts {

class TrikServoMotor : public interpreterBase::robotModel::robotParts::Motor
{
	Q_OBJECT
	Q_CLASSINFO("name", "servo")
	Q_CLASSINFO("friendlyName", tr("Servo Motor"))

public:
	TrikServoMotor(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port);
};

}
}
}
