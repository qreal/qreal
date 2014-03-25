#include "nxtDisplay.h"

using namespace nxtKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

NxtDisplay::NxtDisplay(DeviceInfo const &info, PortInfo const &port)
	: interpreterBase::robotModel::robotParts::Display(info, port)
{
}

void NxtDisplay::doConfiguration()
{
}
