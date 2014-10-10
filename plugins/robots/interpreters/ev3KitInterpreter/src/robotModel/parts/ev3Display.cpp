#include "ev3Display.h"

using namespace ev3KitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

Ev3Display::Ev3Display(DeviceInfo const &info, PortInfo const &port)
	: interpreterBase::robotModel::robotParts::Display(info, port)
{
}
