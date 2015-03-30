#include "ev3Kit/robotModel/parts/ev3Speaker.h"

using namespace ev3::robotModel::parts;
using namespace kitBase::robotModel;

Ev3Speaker::Ev3Speaker(const DeviceInfo &info, const PortInfo &port)
	: kitBase::robotModel::robotParts::Speaker(info, port)
{
}

void Ev3Speaker::beep(unsigned time)
{
	playTone(70, 1000, time);
}
