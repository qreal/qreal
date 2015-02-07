#include "trikGamepadWheel.h"

using namespace trikKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

TrikGamepadWheel::TrikGamepadWheel(const DeviceInfo &info, const PortInfo &port)
	: robotParts::ScalarSensor(info, port)
{
}
