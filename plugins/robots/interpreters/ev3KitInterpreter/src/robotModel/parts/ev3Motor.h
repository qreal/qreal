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
	Ev3Motor(interpreterBase::robotModel::DeviceInfo const &info, interpreterBase::robotModel::PortInfo const &port);

	virtual void on(int speed);
	virtual void stop() override;
	virtual void off() override;
};

}
}
}
