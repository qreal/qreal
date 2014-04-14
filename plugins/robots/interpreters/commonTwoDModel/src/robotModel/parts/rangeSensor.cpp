#include "interpreterBase/robotModel/robotParts/rangeSensor.h"

using namespace interpreterBase::robotModel;
using namespace robotParts;

RangeSensor::RangeSensor(DeviceInfo const &info, PortInfo const &port)
	: ScalarSensor(info, port)
{
}
