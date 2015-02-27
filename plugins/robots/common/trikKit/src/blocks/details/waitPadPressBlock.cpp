#include "waitPadPressBlock.h"

#include "trikKit/robotModel/parts/trikGamepadPadPressSensor.h"

using namespace trik::blocks::details;

WaitPadPressBlock::WaitPadPressBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: WaitForSensorBlock(robotModel)
{
}

void WaitPadPressBlock::responseSlot(int reading)
{
	if (reading == 1) {
		stop();
	}
}

QString WaitPadPressBlock::port()
{
	const int result = eval<int>("Pad");
	switch (result) {
	case 1:
		return "GamepadPad1PressedPort";
	case 2:
		return "GamepadPad2PressedPort";
	}

	return "";
}

kitBase::robotModel::DeviceInfo WaitPadPressBlock::device() const
{
	return kitBase::robotModel::DeviceInfo::create<robotModel::parts::TrikGamepadPadPressSensor>();
}
