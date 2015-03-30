#include "kitBase/robotModel/robotParts/colorSensor.h"

using namespace kitBase::robotModel;
using namespace robotParts;

ColorSensor::ColorSensor(const DeviceInfo &info, const PortInfo &port)
	: ScalarSensor(info, port)
{
}
