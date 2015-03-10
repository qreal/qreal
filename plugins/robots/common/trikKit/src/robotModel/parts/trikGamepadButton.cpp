#include "trikKit/robotModel/parts/trikGamepadButton.h"

using namespace trik::robotModel::parts;
using namespace kitBase::robotModel;

TrikGamepadButton::TrikGamepadButton(const DeviceInfo &info, const PortInfo &port)
	: robotParts::Button(info, port)
{
}
