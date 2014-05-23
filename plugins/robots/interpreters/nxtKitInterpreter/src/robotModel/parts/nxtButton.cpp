#include "nxtButton.h"

using namespace nxtKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

NxtButton::NxtButton(DeviceInfo const &info, PortInfo const &port)
	: interpreterBase::robotModel::robotParts::Button(info, port)
{
}

void NxtButton::doConfiguration()
{
	configurationCompleted(true);
}
