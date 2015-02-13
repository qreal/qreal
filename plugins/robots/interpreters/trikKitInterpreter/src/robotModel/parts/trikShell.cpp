#include "trikShell.h"

using namespace trikKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

TrikShell::TrikShell(const DeviceInfo &info, const PortInfo &port)
	: interpreterBase::robotModel::robotParts::Device(info, port)
{
}
