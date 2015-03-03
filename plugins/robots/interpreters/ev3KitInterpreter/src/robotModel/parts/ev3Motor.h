#pragma once

#include <interpreterBase/robotModel/robotParts/motor.h>

namespace ev3KitInterpreter {
namespace robotModel {
namespace parts {

class Ev3Motor : public interpreterBase::robotModel::robotParts::Motor
{
	Q_OBJECT
	Q_CLASSINFO("name", "ev3Motor")
	Q_CLASSINFO("friendlyName", tr("Motor"))
	Q_CLASSINFO("direction", "output")

public:
	Ev3Motor(const interpreterBase::robotModel::DeviceInfo &info, const interpreterBase::robotModel::PortInfo &port);

	virtual void on(int speed);
	virtual void stop() override;
	virtual void off() override;
};

}
}
}
