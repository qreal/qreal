#include "interpreterBase/robotModel/robotParts/lightSensor.h"

using namespace interpreterBase::robotModel;
using namespace robotParts;

QString LightSensor::friendlyName()
{
	return tr("Light sensor");
}

DeviceInfo::Direction LightSensor::direction()
{
	return DeviceInfo::input;
}

LightSensor::LightSensor(DeviceInfo const &info, PortInfo const &port)
	: ScalarSensor(info, port)
{
}
