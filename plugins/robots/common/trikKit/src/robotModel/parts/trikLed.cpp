#include "trikKit/robotModel/parts/trikLed.h"

using namespace trik::robotModel::parts;
using namespace kitBase::robotModel;

TrikLed::TrikLed(const DeviceInfo &info, const PortInfo &port)
	: kitBase::robotModel::robotParts::Device(info, port)
{
}
