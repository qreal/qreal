#include "colorSensor.h"

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

ColorSensor::ColorSensor(DeviceInfo const &info, PortInfo const &port)
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
