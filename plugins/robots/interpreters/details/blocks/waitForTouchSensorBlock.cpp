#include "waitForTouchSensorBlock.h"

#include "../../sensorConstants.h"

using namespace qReal;
using namespace interpreters;
using namespace interpreters::robots;
using namespace interpreters::robots::details::blocks;

WaitForTouchSensorBlock::WaitForTouchSensorBlock(details::RobotModel * const robotModel)
	: WaitForSensorBlock(robotModel, robots::enums::sensorType::touchBoolean)
{
}

interpreters::robots::details::robotParts::Sensor *WaitForTouchSensorBlock::sensor() const
{
	return mRobotModel->touchSensor(mPort);
}

QString WaitForTouchSensorBlock::name() const
{
	return tr("Touch sensor");
}

void WaitForTouchSensorBlock::responseSlot(int reading)
{
	if (reading == 1) {
		stop();
	}
}
