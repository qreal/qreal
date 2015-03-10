#include "waitGamepadDisconnectBlock.h"

#include "trikKit/robotModel/parts/trikGamepadConnectionIndicator.h"

using namespace trik::blocks::details;

WaitGamepadDisconnectBlock::WaitGamepadDisconnectBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: WaitForSensorBlock(robotModel)
{
}

void WaitGamepadDisconnectBlock::responseSlot(int reading)
{
	if (reading  == 0) {
		stop();
	}
}

QString WaitGamepadDisconnectBlock::port()
{
	return "GamepadConnectionIndicatorPort";
}

kitBase::robotModel::DeviceInfo WaitGamepadDisconnectBlock::device() const
{
	return kitBase::robotModel::DeviceInfo::create<robotModel::parts::TrikGamepadConnectionIndicator>();
}
