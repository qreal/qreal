#include "motor.h"

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

Motor::Motor(DeviceInfo const &info, PortInfo const &port)
	: interpreterBase::robotModel::robotParts::Motor(info, port)
{
}

void Motor::on(int speed, bool breakMode)
{
	on(speed, 0, breakMode);
}

void Motor::on(int speed, long unsigned int degrees, bool breakMode)
{
	Q_UNUSED(speed)
	Q_UNUSED(degrees)
	Q_UNUSED(breakMode)
}

void Motor::stop(bool breakMode)
{
	on(0, 0, breakMode);
}

void Motor::off()
{
}

void Motor::doConfiguration()
{
	/// @todo Remove this to base class.
	configurationCompleted(true);
}
