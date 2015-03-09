#include "waitGamepadWheelBlock.h"

#include "robotModel/parts/trikGamepadWheel.h"

using namespace trikKitInterpreter::blocks::details;

WaitGamepadWheelBlock::WaitGamepadWheelBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
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

interpreterBase::robotModel::DeviceInfo WaitGamepadWheelBlock::device() const
{
	return interpreterBase::robotModel::DeviceInfo::create<robotModel::parts::TrikGamepadWheel>();
}
