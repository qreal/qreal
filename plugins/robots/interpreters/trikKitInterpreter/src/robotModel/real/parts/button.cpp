#include "button.h"

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

Button::Button(DeviceInfo const &info, PortInfo const &port)
	: TrikButton(info, port)
{
}

void Button::read()
{
	emit response(true, true, true, true, true, true, true);
}
