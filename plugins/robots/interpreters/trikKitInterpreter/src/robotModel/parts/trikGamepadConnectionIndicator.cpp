#include "trikGamepadConnectionIndicator.h"

using namespace trikKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

TrikGamepadConnectionIndicator::TrikGamepadConnectionIndicator(const DeviceInfo &info, const PortInfo &port)
	: robotParts::ScalarSensor(info, port)
{
}
