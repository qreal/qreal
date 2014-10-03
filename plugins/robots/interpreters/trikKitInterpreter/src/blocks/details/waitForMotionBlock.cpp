#include "waitForMotionBlock.h"

#include "robotModel/parts/trikMotionSensor.h"

using namespace trikKitInterpreter::blocks::details;

WaitForMotionBlock::WaitForMotionBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: WaitForSensorBlock(robotModel)
{
}

void WaitForMotionBlock::responseSlot(int reading)
{
	if (reading > 0) {
		stop();
	}
}

QString WaitForMotionBlock::port() const
{
	return "F1";
}

interpreterBase::robotModel::DeviceInfo WaitForMotionBlock::device() const
{
	return interpreterBase::robotModel::DeviceInfo::create<robotModel::parts::TrikMotionSensor>();
}
