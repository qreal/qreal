#include "waitGamepadConnectBlock.h"

#include "trikKit/robotModel/parts/trikGamepadConnectionIndicator.h"

using namespace trik::blocks::details;

WaitGamepadConnectBlock::WaitGamepadConnectBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: WaitForSensorBlock(robotModel)
{
}

void WaitGamepadConnectBlock::responseSlot(int reading)
{
	if (reading  == 1) {
		stop();
	}
}

QString WaitGamepadConnectBlock::port()
{
	return "GamepadConnectionIndicatorPort";
}

kitBase::robotModel::DeviceInfo WaitGamepadConnectBlock::device() const
{
	return kitBase::robotModel::DeviceInfo::create<robotModel::parts::TrikGamepadConnectionIndicator>();
}
