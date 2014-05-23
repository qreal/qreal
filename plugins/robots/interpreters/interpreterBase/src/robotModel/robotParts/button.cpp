#include "interpreterBase/robotModel/robotParts/button.h"

using namespace interpreterBase::robotModel::robotParts;

Button::Button(DeviceInfo const &info, PortInfo const &port)
	: ScalarSensor(info, port)
{
}

void Button::doConfiguration()
{
	emit configured(true);
}
