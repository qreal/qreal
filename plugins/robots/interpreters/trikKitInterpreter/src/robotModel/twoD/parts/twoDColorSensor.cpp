#include "twoDColorSensor.h"

using namespace trik::robotModel::twoD::parts;
using namespace kitBase::robotModel;

ColorSensor::ColorSensor(const DeviceInfo &info, const PortInfo &port)
	: robotModel::parts::TrikColorSensor(info, port)
{
}

void ColorSensor::init()
{
}

void ColorSensor::read()
{
	emit newData({0, 0, 0});
}
