#include "nxtKit/robotModel/parts/nxtMotor.h"

using namespace nxt::robotModel::parts;
using namespace kitBase::robotModel;

NxtMotor::NxtMotor(const DeviceInfo &info, const PortInfo &port)
	: robotParts::Motor(info, port)
{
}

void NxtMotor::on(int speed)
{
	on(speed, true);
}

void NxtMotor::stop()
{
	stop(true);
}

void NxtMotor::off()
{
	stop(false);
}

void NxtMotor::on(int speed, bool breakMode)
{
	on(speed, 0, breakMode);
}

void NxtMotor::on(int speed, long unsigned int degrees, bool breakMode)
{
	interpreterBase::robotModel::robotParts::Motor::on(speed);
	Q_UNUSED(speed);
	Q_UNUSED(degrees);
	Q_UNUSED(breakMode);
}

void NxtMotor::stop(bool breakMode)
{
	Q_UNUSED(breakMode);
}
