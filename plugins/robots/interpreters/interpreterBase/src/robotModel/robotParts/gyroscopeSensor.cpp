#include "interpreterBase/robotModel/robotParts/gyroscopeSensor.h"

using namespace interpreterBase::robotModel;
using namespace robotParts;

GyroscopeSensor::GyroscopeSensor(DeviceInfo const &info, PortInfo const &port)
	: ScalarSensor(info, port)
{
}
