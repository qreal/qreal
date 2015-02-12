#include "trikDisplay.h"

using namespace trikKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

TrikDisplay::TrikDisplay(const DeviceInfo &info, const PortInfo &port)
	: interpreterBase::robotModel::robotParts::Display(info, port)
{
}
