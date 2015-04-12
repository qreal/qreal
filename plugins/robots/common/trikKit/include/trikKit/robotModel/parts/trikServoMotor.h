#pragma once

#include <kitBase/robotModel/robotParts/motor.h>

namespace trik {
namespace robotModel {
namespace parts {

class TrikServoMotor : public kitBase::robotModel::robotParts::Motor
{
	Q_OBJECT
	Q_CLASSINFO("name", "servo")
	Q_CLASSINFO("friendlyName", tr("Servo Motor"))

public:
	TrikServoMotor(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port);
};

}
}
}
