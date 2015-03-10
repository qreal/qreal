#include "kitBase/robotModel/robotParts/lightSensor.h"

using namespace kitBase::robotModel;
using namespace robotParts;

LightSensor::LightSensor(const DeviceInfo &info, const PortInfo &port)
	: ScalarSensor(info, port)
{
}
