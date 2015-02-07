#include "waitPadPressBlock.h"

#include "robotModel/parts/trikGamepadPadPressSensor.h"

using namespace trikKitInterpreter::blocks::details;

WaitPadPressBlock::WaitPadPressBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
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
	int const result = eval<int>("Pad");
	switch (result) {
	case 1:
		return "GamepadPad1PressedPort";
	case 2:
		return "GamepadPad2PressedPort";
	}

	return "";
}

interpreterBase::robotModel::DeviceInfo WaitPadPressBlock::device() const
{
	return interpreterBase::robotModel::DeviceInfo::create<robotModel::parts::TrikGamepadPadPressSensor>();
}
