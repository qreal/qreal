#include "nxtKit/robotModel/parts/nxtSpeaker.h"

using namespace nxt::robotModel::parts;
using namespace kitBase::robotModel;

NxtSpeaker::NxtSpeaker(const DeviceInfo &info, const PortInfo &port)
	: robotParts::Speaker(info, port)
{
}

void NxtSpeaker::beep(unsigned time)
{
	playTone(1000, time);
}
