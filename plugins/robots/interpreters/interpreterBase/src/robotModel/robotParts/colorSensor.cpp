#include "interpreterBase/robotModel/robotParts/colorSensor.h"

using namespace interpreterBase::robotModel;
using namespace robotParts;

ColorSensor::ColorSensor(DeviceInfo const &info, PortInfo const &port)
	: ScalarSensor(info, port)
{
}
