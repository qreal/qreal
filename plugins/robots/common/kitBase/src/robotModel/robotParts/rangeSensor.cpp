#include "kitBase/robotModel/robotParts/rangeSensor.h"

using namespace kitBase::robotModel;
using namespace robotParts;

RangeSensor::RangeSensor(const DeviceInfo &info, const PortInfo &port)
	: ScalarSensor(info, port)
{
}
