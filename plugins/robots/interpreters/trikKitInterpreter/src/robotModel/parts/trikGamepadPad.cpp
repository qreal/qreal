#include "trikGamepadPad.h"

using namespace trikKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

TrikGamepadPad::TrikGamepadPad(const DeviceInfo &info, const PortInfo &port)
	: robotParts::VectorSensor(info, port)
{
}
