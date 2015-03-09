#include "interpreterBase/robotModel/robotParts/gyroscopeSensor.h"

using namespace interpreterBase::robotModel;
using namespace robotParts;

GyroscopeSensor::GyroscopeSensor(const DeviceInfo &info, const PortInfo &port)
	: ScalarSensor(info, port)
{
}
