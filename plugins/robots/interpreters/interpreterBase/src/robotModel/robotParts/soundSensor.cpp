#include "interpreterBase/robotModel/robotParts/soundSensor.h"

using namespace interpreterBase::robotModel;
using namespace robotParts;

SoundSensor::SoundSensor(const DeviceInfo &info, const PortInfo &port)
	: ScalarSensor(info, port)
{
}
