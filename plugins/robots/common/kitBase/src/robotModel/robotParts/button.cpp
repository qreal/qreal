#include "kitBase/robotModel/robotParts/button.h"

using namespace kitBase::robotModel::robotParts;

Button::Button(const DeviceInfo &info, const PortInfo &port)
	: ScalarSensor(info, port)
{
}
