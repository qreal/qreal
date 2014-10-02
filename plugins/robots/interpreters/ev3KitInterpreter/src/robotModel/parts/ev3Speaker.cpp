#include "ev3Speaker.h"
#include "commandConstants.h"

using namespace ev3KitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

Ev3Speaker::Ev3Speaker(DeviceInfo const &info, PortInfo const &port)
	: interpreterBase::robotModel::robotParts::Speaker(info, port)
{
}

void Ev3Speaker::beep(unsigned time)
{
	playTone(70, 1000, time);
}
