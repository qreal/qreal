#include "interpreterBase/robotModel/robotParts/lightSensor.h"

using namespace interpreterBase::robotModel;
using namespace robotParts;

LightSensor::LightSensor(DeviceInfo const &info, PortInfo const &port)
	: ScalarSensor(info, port)
{
}
