#include "interpreterBase/robotModel/robotParts/soundSensor.h"

using namespace interpreterBase::robotModel;
using namespace robotParts;

QString SoundSensor::friendlyName()
{
	return tr("Sound sensor");
}

DeviceInfo::Direction SoundSensor::direction()
{
	return DeviceInfo::input;
}

SoundSensor::SoundSensor(DeviceInfo const &info, PortInfo const &port)
	: ScalarSensor(info, port)
{
}
