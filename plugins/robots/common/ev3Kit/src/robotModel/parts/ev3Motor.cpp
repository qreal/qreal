#include "ev3Kit/robotModel/parts/ev3Motor.h"

using namespace ev3::robotModel::parts;
using namespace kitBase::robotModel;

Ev3Motor::Ev3Motor(const DeviceInfo &info, const kitBase::robotModel::PortInfo &port)
	: kitBase::robotModel::robotParts::Motor(info, port)
{
}

void Ev3Motor::on(int speed)
{
	Motor::on(speed);
}

void Ev3Motor::stop()
{
}

void Ev3Motor::off()
{
}
