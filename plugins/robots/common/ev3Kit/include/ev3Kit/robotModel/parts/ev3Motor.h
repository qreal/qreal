#pragma once

#include <kitBase/robotModel/robotParts/motor.h>

namespace ev3 {
namespace robotModel {
namespace parts {

class Ev3Motor : public kitBase::robotModel::robotParts::Motor
{
	Q_OBJECT
	Q_CLASSINFO("name", "ev3Motor")
	Q_CLASSINFO("friendlyName", tr("Motor"))
	Q_CLASSINFO("direction", "output")

public:
	Ev3Motor(const kitBase::robotModel::DeviceInfo &info, const kitBase::robotModel::PortInfo &port);

	virtual void on(int speed);
	virtual void stop() override;
	virtual void off() override;
};

}
}
}
