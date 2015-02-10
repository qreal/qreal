#include "ev3Display.h"

using namespace ev3KitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

Ev3Display::Ev3Display(const DeviceInfo &info, const PortInfo &port)
	: interpreterBase::robotModel::robotParts::Display(info, port)
{
}
