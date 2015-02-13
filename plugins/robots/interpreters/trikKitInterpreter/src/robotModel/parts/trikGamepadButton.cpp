#include "trikGamepadButton.h"

using namespace trikKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

TrikGamepadButton::TrikGamepadButton(const DeviceInfo &info, const PortInfo &port)
	: robotParts::Button(info, port)
{
}
