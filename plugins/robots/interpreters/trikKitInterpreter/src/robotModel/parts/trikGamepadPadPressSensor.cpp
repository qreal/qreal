#include "trikGamepadPadPressSensor.h"

using namespace trikKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

TrikGamepadPadPressSensor::TrikGamepadPadPressSensor(const DeviceInfo &info, const PortInfo &port)
	: robotParts::Button(info, port)
{
}
