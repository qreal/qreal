#include "trikKit/robotModel/parts/trikGamepadPadPressSensor.h"

using namespace trik::robotModel::parts;
using namespace kitBase::robotModel;

TrikGamepadPadPressSensor::TrikGamepadPadPressSensor(const DeviceInfo &info, const PortInfo &port)
	: robotParts::Button(info, port)
{
}
