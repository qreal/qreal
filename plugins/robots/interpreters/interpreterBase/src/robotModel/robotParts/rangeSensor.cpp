#include "interpreterBase/robotModel/robotParts/rangeSensor.h"

using namespace interpreterBase::robotModel;
using namespace robotParts;

RangeSensor::RangeSensor(const DeviceInfo &info, const PortInfo &port)
	: ScalarSensor(info, port)
{
}
