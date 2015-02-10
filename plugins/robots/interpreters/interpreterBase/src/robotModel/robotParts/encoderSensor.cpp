#include "interpreterBase/robotModel/robotParts/encoderSensor.h"

using namespace interpreterBase::robotModel;
using namespace robotParts;

EncoderSensor::EncoderSensor(const DeviceInfo &info, const PortInfo &port)
	: ScalarSensor(info, port)
{
}
