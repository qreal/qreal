#include "waitGamepadWheelBlock.h"

#include "trikKit/robotModel/parts/trikGamepadWheel.h"

using namespace trik::blocks::details;

WaitGamepadWheelBlock::WaitGamepadWheelBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: WaitForSensorBlock(robotModel)
{
}

void WaitGamepadWheelBlock::responseSlot(int reading)
{
	const int result = eval<int>("Angle");
	if (!errorsOccured()) {
		processResponce(reading, result);
	}
}

QString WaitGamepadWheelBlock::port()
{
	return "gamepadWheel";
}

kitBase::robotModel::DeviceInfo WaitGamepadWheelBlock::device() const
{
	return kitBase::robotModel::DeviceInfo::create<robotModel::parts::TrikGamepadWheel>();
}
