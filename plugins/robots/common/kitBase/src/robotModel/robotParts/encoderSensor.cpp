#include "kitBase/robotModel/robotParts/encoderSensor.h"

using namespace kitBase::robotModel;
using namespace robotParts;

EncoderSensor::EncoderSensor(const DeviceInfo &info, const PortInfo &port)
	: ScalarSensor(info, port)
{
}
