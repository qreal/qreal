#include "kitBase/robotModel/robotParts/soundSensor.h"

using namespace kitBase::robotModel;
using namespace robotParts;

SoundSensor::SoundSensor(const DeviceInfo &info, const PortInfo &port)
	: ScalarSensor(info, port)
{
}
