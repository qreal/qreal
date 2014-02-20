#include "interpreterBase/robotModel/robotParts/encoderSensor.h"

using namespace interpreterBase::robotModel;
using namespace robotParts;

QString EncoderSensor::friendlyName()
{
	return tr("Encoder");
}

DeviceInfo::Direction EncoderSensor::direction()
{
	return DeviceInfo::input;
}

EncoderSensor::EncoderSensor(DeviceInfo const &info, PortInfo const &port)
	: ScalarSensor(info, port)
{
}
