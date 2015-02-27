#include "trikKit/robotModel/parts/trikGamepadPad.h"

using namespace trik::robotModel::parts;
using namespace kitBase::robotModel;

TrikGamepadPad::TrikGamepadPad(const DeviceInfo &info, const PortInfo &port)
	: robotParts::VectorSensor(info, port)
{
}
