#include "interpreterBase/robotModel/robotParts/motor.h"

using namespace interpreterBase::robotModel::robotParts;

Motor::Motor(PluggableDeviceInfo const &info, interpreterBase::robotModel::PortInfo const &port)
	: PluggableDevice(info, port)
{
}

void Motor::on(int speed, bool breakMode)
{
	Q_UNUSED(speed);
	Q_UNUSED(breakMode);
}

void Motor::on(int speed, long unsigned int degrees, bool breakMode)
{
	Q_UNUSED(speed);
	Q_UNUSED(degrees);
	Q_UNUSED(breakMode);
}

void Motor::stop(bool breakMode)
{
	Q_UNUSED(breakMode);
}

void Motor::off()
{
}
