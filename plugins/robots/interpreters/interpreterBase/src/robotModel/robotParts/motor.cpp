#include "interpreterBase/robotModel/robotParts/motor.h"

using namespace interpreterBase::robotModel;
using namespace robotParts;

Motor::Motor(DeviceInfo const &info, interpreterBase::robotModel::PortInfo const &port)
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
