#include "interpreterBase/robotModel/robotParts/motor.h"

using namespace interpreterBase::robotModel::robotParts;

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
