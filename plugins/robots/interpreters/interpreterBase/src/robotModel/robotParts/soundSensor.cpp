#include "interpreterBase/robotModel/robotParts/soundSensor.h"

using namespace interpreterBase::robotModel;
using namespace robotParts;

SoundSensor::SoundSensor(DeviceInfo const &info, PortInfo const &port)
	: ScalarSensor(info, port)
{
}
