#pragma once

#include <interpreterBase/robotModel/robotParts/motor.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace parts {

class TrikPowerMotor : public interpreterBase::robotModel::robotParts::Motor
{
	Q_OBJECT
	Q_CLASSINFO("name", "power")
	Q_CLASSINFO("friendlyName", tr("Power Motor"))

public:
	TrikPowerMotor(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port);
};

}
}
}
