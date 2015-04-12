#include "waitForMotionBlock.h"

#include "trikKit/robotModel/parts/trikMotionSensor.h"

using namespace trik::blocks::details;

WaitForMotionBlock::WaitForMotionBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: WaitForSensorBlock(robotModel)
{
}

void WaitForMotionBlock::responseSlot(int reading)
{
	if (reading > 0) {
		stop();
	}
}

QString WaitForMotionBlock::port()
{
	return "F1";
}

kitBase::robotModel::DeviceInfo WaitForMotionBlock::device() const
{
	return kitBase::robotModel::DeviceInfo::create<robotModel::parts::TrikMotionSensor>();
}
