#include "nxtButtons.h"

using namespace nxtKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

NxtButtons::NxtButtons(DeviceInfo const &info, PortInfo const &port)
	: interpreterBase::robotModel::robotParts::Buttons(info, port)
{
}

void NxtButtons::doConfiguration()
{
	configurationCompleted(true);
}
