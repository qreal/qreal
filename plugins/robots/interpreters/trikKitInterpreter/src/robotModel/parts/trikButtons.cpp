#include "trikButtons.h"

using namespace trikKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

TrikButtons::TrikButtons(DeviceInfo const &info, PortInfo const &port)
	: interpreterBase::robotModel::robotParts::Buttons(info, port)
{
}

void TrikButtons::doConfiguration()
{
	configurationCompleted(true);
}
