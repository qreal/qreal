#include "trikKit/robotModel/parts/trikGamepadConnectionIndicator.h"

using namespace trik::robotModel::parts;
using namespace kitBase::robotModel;

TrikGamepadConnectionIndicator::TrikGamepadConnectionIndicator(const DeviceInfo &info, const PortInfo &port)
	: robotParts::ScalarSensor(info, port)
{
}
