#include "nxtDisplay.h"

using namespace nxtKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

NxtDisplay::NxtDisplay(const DeviceInfo &info, const PortInfo &port)
	: interpreterBase::robotModel::robotParts::Display(info, port)
{
}
