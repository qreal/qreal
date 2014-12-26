#include "trikShell.h"

using namespace trikKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

TrikShell::TrikShell(DeviceInfo const &info, PortInfo const &port)
	: interpreterBase::robotModel::robotParts::Device(info, port)
{
}
