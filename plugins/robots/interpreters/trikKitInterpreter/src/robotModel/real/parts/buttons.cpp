#include "buttons.h"

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

Buttons::Buttons(DeviceInfo const &info, PortInfo const &port)
	: TrikButtons(info, port)
{
}

void Buttons::read()
{
	emit response(true, true, true, true, true, true);
}
