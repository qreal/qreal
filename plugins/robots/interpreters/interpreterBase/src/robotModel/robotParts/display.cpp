#include "interpreterBase/robotModel/robotParts/display.h"

using namespace interpreterBase::robotModel;
using namespace robotParts;

DeviceInfo::Direction Display::direction()
{
	return DeviceInfo::output;
}

Display::Display(DeviceInfo const &info, PortInfo const &port)
	: Device(info, port)
{
}
