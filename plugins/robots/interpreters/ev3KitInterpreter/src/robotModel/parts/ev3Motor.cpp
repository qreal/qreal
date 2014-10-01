#include "ev3Motor.h"

using namespace ev3KitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

Ev3Motor::Ev3Motor(DeviceInfo const &info, interpreterBase::robotModel::PortInfo const &port)
	: interpreterBase::robotModel::robotParts::Motor(info, port)
{
}

void Ev3Motor::on(int speed)
{
	on(speed, true);
}

void Ev3Motor::stop()
{
	stop(true);
}

void Ev3Motor::off()
{
	stop(false);
}

void Ev3Motor::on(int speed, bool breakMode)
{
	on(speed, 0, breakMode);
}

void Ev3Motor::on(int speed, long unsigned int degrees, bool breakMode)
{
	Q_UNUSED(speed);
	Q_UNUSED(degrees);
	Q_UNUSED(breakMode);
}

void Ev3Motor::stop(bool breakMode)
{
	Q_UNUSED(breakMode);
}
