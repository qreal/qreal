#include "waitGamepadConnectBlock.h"

#include "robotModel/parts/trikGamepadConnectionIndicator.h"

using namespace trikKitInterpreter::blocks::details;

WaitGamepadConnectBlock::WaitGamepadConnectBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
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

interpreterBase::robotModel::DeviceInfo WaitGamepadConnectBlock::device() const
{
	return interpreterBase::robotModel::DeviceInfo::create<robotModel::parts::TrikGamepadConnectionIndicator>();
}
