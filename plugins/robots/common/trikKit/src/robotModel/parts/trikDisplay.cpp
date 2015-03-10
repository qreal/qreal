#include "trikKit/robotModel/parts/trikDisplay.h"

using namespace trik::robotModel::parts;
using namespace kitBase::robotModel;

TrikDisplay::TrikDisplay(const DeviceInfo &info, const PortInfo &port)
	: kitBase::robotModel::robotParts::Display(info, port)
{
}
