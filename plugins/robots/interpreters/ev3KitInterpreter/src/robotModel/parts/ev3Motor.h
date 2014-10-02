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

	void on(int speed) override;
	void stop() override;
	void off() override;

	virtual void on(int speed, bool breakMode);
	virtual void on(int speed, long unsigned int degrees, bool breakMode);
	virtual void stop(bool breakMode);
};

}
}
}
