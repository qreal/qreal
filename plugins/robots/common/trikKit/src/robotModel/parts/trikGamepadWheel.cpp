#include "trikKit/robotModel/parts/trikGamepadWheel.h"

using namespace trik::robotModel::parts;
using namespace kitBase::robotModel;

TrikGamepadWheel::TrikGamepadWheel(const DeviceInfo &info, const PortInfo &port)
	: robotParts::ScalarSensor(info, port)
{
}
