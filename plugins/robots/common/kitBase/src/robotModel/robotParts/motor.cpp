#include "kitBase/robotModel/robotParts/motor.h"

using namespace kitBase::robotModel;
using namespace robotParts;

Motor::Motor(const DeviceInfo &info, const kitBase::robotModel::PortInfo &port)
	: Device(info, port)
{
}

void Motor::on(int speed)
{
	Q_UNUSED(speed);
}

void Motor::stop()
{
}

void Motor::off()
{
}
