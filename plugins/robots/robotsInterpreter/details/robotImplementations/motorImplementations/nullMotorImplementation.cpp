#include "nullMotorImplementation.h"

using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::motorImplementations;

NullMotorImplementation::NullMotorImplementation(int const port)
	: AbstractMotorImplementation(port)
{
}

void NullMotorImplementation::on(int speed, bool breakMode)
{
	Q_UNUSED(speed)
	Q_UNUSED(breakMode)
}

void NullMotorImplementation::on(int speed, long unsigned int degrees, bool breakMode)
{
	Q_UNUSED(speed)
	Q_UNUSED(degrees)
	Q_UNUSED(breakMode)
}

void NullMotorImplementation::stop(bool breakMode)
{
	Q_UNUSED(breakMode)
}

void NullMotorImplementation::off()
{
}

void NullMotorImplementation::resetMotorPosition(bool relative)
{
	Q_UNUSED(relative);
}
