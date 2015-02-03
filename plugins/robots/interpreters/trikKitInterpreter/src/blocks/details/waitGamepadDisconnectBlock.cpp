#include "waitGamepadDisconnectBlock.h"

#include "robotModel/parts/trikGamepadConnectionIndicator.h"

using namespace trikKitInterpreter::blocks::details;

WaitGamepadDisconnectBlock::WaitGamepadDisconnectBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
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

interpreterBase::robotModel::DeviceInfo WaitGamepadDisconnectBlock::device() const
{
	return interpreterBase::robotModel::DeviceInfo::create<robotModel::parts::TrikGamepadConnectionIndicator>();
}
