#include "nullMotorImplementation.h"
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::motorImplementations;

NullMotorImplementation::NullMotorImplementation(int const port)
	: AbstractMotorImplementation(port)
{
}

void NullMotorImplementation::on(int speed)
{
	Q_UNUSED(speed);
}

void NullMotorImplementation::on(int speed, long unsigned int degrees)
{
	Q_UNUSED(speed);
	Q_UNUSED(degrees);
}

void NullMotorImplementation::resetMotorPosition(bool relative)
{
	Q_UNUSED(relative);
}
