#include "interpreterBase/robotModel/robotParts/lightSensor.h"

using namespace interpreterBase::robotModel;
using namespace robotParts;

LightSensor::LightSensor(const DeviceInfo &info, const PortInfo &port)
	: ScalarSensor(info, port)
{
}
