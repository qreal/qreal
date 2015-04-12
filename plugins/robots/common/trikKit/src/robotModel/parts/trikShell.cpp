#include "trikKit/robotModel/parts/trikShell.h"

using namespace trik::robotModel::parts;
using namespace kitBase::robotModel;

TrikShell::TrikShell(const DeviceInfo &info, const PortInfo &port)
	: kitBase::robotModel::robotParts::Device(info, port)
{
}
