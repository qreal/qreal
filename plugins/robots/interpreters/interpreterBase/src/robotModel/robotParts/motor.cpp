#include "interpreterBase/robotModel/robotParts/motor.h"

using namespace interpreterBase::robotModel;
using namespace robotParts;

Motor::Motor(const DeviceInfo &info, const interpreterBase::robotModel::PortInfo &port)
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
