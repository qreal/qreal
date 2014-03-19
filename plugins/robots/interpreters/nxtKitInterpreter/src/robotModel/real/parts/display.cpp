#include "display.h"

using namespace nxtKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

Display::Display(DeviceInfo const &info, PortInfo const &port)
	: interpreterBase::robotModel::robotParts::Display(info, port)
{
}

void Display::doConfiguration()
{
}
