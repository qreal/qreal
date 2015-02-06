#include "interpreterBase/robotModel/robotParts/button.h"

using namespace interpreterBase::robotModel::robotParts;

Button::Button(const DeviceInfo &info, const PortInfo &port)
	: ScalarSensor(info, port)
{
}
