#include "buttons.h"

using namespace nxtKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

Buttons::Buttons(DeviceInfo const &info, PortInfo const &port)
	: NxtButtons(info, port)
{
}

void Buttons::read()
{
	emit response(true, true, true, true);
}
