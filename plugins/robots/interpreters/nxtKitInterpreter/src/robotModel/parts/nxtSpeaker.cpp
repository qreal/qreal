#include "nxtSpeaker.h"
#include "commandConstants.h"

using namespace nxtKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

NxtSpeaker::NxtSpeaker(DeviceInfo const &info, PortInfo const &port)
	: interpreterBase::robotModel::robotParts::Speaker(info, port)
{
}

void NxtSpeaker::beep(unsigned time)
{
	playTone(1000, time);
}
