#include "interpreterBase/robotModel/robotParts/colorSensor.h"

using namespace interpreterBase::robotModel;
using namespace robotParts;

ColorSensor::ColorSensor(const DeviceInfo &info, const PortInfo &port)
	: ScalarSensor(info, port)
{
}
