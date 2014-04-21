#include "trikDisplay.h"

using namespace trikKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

TrikDisplay::TrikDisplay(DeviceInfo const &info, PortInfo const &port)
	: interpreterBase::robotModel::robotParts::Display(info, port)
{
}

void TrikDisplay::doConfiguration()
{
	configurationCompleted(true);
}
