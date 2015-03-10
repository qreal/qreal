#include "button.h"

using namespace nxt::robotModel::real::parts;
using namespace kitBase::robotModel;

Button::Button(const DeviceInfo &info, const PortInfo &port)
	: robotParts::Button(info, port)
{
}

void Button::read()
{
	emit newData(1);
}
