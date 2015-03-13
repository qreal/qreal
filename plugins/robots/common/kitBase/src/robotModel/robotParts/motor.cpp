#include "kitBase/robotModel/robotParts/motor.h"

using namespace kitBase::robotModel;
using namespace robotParts;

Motor::Motor(const DeviceInfo &info, const kitBase::robotModel::PortInfo &port)
	: Device(info, port)
	, mSpeed(0)
{
}

int Motor::power() const
{
	return mSpeed;
}

void Motor::on(int speed)
{
	mSpeed = speed;
}

void Motor::stop()
{
}

void Motor::off()
{
}
