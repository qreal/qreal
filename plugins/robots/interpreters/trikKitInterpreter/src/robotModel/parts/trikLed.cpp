#include "trikLed.h"


using namespace trikKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

TrikLed::TrikLed(DeviceInfo const &info, PortInfo const &port)
	: interpreterBase::robotModel::robotParts::Device(info, port)
{
}
