#include "button.h"

using namespace nxtKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

Button::Button(const DeviceInfo &info, const PortInfo &port)
	: robotParts::Button(info, port)
{
}

void Button::read()
{
	emit newData(1);
}
