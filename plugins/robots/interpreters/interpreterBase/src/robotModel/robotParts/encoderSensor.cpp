#include "interpreterBase/robotModel/robotParts/encoderSensor.h"

using namespace interpreterBase::robotModel;
using namespace robotParts;

EncoderSensor::EncoderSensor(DeviceInfo const &info, PortInfo const &port)
	: ScalarSensor(info, port)
{
}

EncoderSensor::~EncoderSensor()
{
}
