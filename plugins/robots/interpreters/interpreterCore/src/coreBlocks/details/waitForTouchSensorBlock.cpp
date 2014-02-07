#include "waitForTouchSensorBlock.h"

//#include "../../sensorConstants.h"

using namespace interpreterCore::coreBlocks::details;

WaitForTouchSensorBlock::WaitForTouchSensorBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: WaitForSensorBlock(robotModel)
{
}

WaitForTouchSensorBlock::~WaitForTouchSensorBlock()
{
}

//interpreters::robots::details::robotParts::Sensor *WaitForTouchSensorBlock::sensor() const
//{
//	return mRobotModel->touchSensor(mPort);
//}

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
