#include "trikButton.h"

using namespace trikKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

TrikButton::TrikButton(DeviceInfo const &info, PortInfo const &port)
	: interpreterBase::robotModel::robotParts::Button(info, port)
{
}

void TrikButton::doConfiguration()
{
	configurationCompleted(true);
}
