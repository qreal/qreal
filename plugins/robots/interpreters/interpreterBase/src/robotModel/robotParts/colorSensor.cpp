#include "interpreterBase/robotModel/robotParts/colorSensor.h"

using namespace interpreterBase::robotModel;
using namespace robotParts;

QString ColorSensor::friendlyName()
{
	return tr("Color sensor");
}

DeviceInfo::Direction ColorSensor::direction()
{
	return DeviceInfo::input;
}

ColorSensor::ColorSensor(DeviceInfo const &info, PortInfo const &port)
	: ScalarSensor(info, port)
{
}
