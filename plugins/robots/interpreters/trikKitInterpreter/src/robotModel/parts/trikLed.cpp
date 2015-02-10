#include "trikLed.h"

using namespace trikKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

TrikLed::TrikLed(const DeviceInfo &info, const PortInfo &port)
	: interpreterBase::robotModel::robotParts::Device(info, port)
{
}
