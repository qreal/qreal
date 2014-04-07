#include "interpreterBase/robotModel/robotParts/motor.h"

using namespace interpreterBase::robotModel;
using namespace robotParts;

Motor::Motor(DeviceInfo const &info, interpreterBase::robotModel::PortInfo const &port)
	: Device(info, port)
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
