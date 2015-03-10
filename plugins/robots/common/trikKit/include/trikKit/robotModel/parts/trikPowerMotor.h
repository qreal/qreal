#pragma once

#include <kitBase/robotModel/robotParts/motor.h>

namespace trik {
namespace robotModel {
namespace parts {

class TrikPowerMotor : public kitBase::robotModel::robotParts::Motor
{
	Q_OBJECT
	Q_CLASSINFO("name", "power")
	Q_CLASSINFO("friendlyName", tr("Power Motor"))

public:
	TrikPowerMotor(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port);
};

}
}
}
