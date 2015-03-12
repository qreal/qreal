#pragma once

#include <kitBase/robotModel/robotParts/motor.h>

namespace nxt {
namespace robotModel {
namespace parts {

class NxtMotor : public kitBase::robotModel::robotParts::Motor
{
	Q_OBJECT
	Q_CLASSINFO("name", "nxtMotor")
	Q_CLASSINFO("friendlyName", tr("Motor"))
	Q_CLASSINFO("direction", "output")

public:
	NxtMotor(const kitBase::robotModel::DeviceInfo &info, const kitBase::robotModel::PortInfo &port);

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
