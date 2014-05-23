#include "button.h"

using namespace nxtKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

Button::Button(DeviceInfo const &info, PortInfo const &port)
	: NxtButton(info, port)
{
}

void Button::read()
{
	emit response(true, true, true, true);
}
