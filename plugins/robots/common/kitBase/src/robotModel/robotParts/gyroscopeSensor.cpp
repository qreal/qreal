#include "kitBase/robotModel/robotParts/gyroscopeSensor.h"

using namespace kitBase::robotModel;
using namespace robotParts;

GyroscopeSensor::GyroscopeSensor(const DeviceInfo &info, const PortInfo &port)
	: ScalarSensor(info, port)
{
}
