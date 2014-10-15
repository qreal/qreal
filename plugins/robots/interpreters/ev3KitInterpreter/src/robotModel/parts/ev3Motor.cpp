#include "ev3Motor.h"

using namespace ev3KitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

Ev3Motor::Ev3Motor(DeviceInfo const &info, interpreterBase::robotModel::PortInfo const &port)
	: interpreterBase::robotModel::robotParts::Motor(info, port)
{
}

void Ev3Motor::on(int speed)
{
	Q_UNUSED(speed);
}

void Ev3Motor::stop()
{
}

void Ev3Motor::off()
{
}
