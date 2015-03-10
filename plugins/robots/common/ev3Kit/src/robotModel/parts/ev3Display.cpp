#include "ev3Kit/robotModel/parts/ev3Display.h"

using namespace ev3::robotModel::parts;
using namespace kitBase::robotModel;

Ev3Display::Ev3Display(const DeviceInfo &info, const PortInfo &port)
	: kitBase::robotModel::robotParts::Display(info, port)
{
}
