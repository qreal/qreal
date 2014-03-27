#include "waitForLightSensorBlock.h"

using namespace interpreterCore::coreBlocks::details;

WaitForLightSensorBlock::WaitForLightSensorBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: WaitForSensorBlock(robotModel)
{
}

WaitForLightSensorBlock::~WaitForLightSensorBlock()
{
}

void WaitForLightSensorBlock::responseSlot(int reading)
{
	int const targetPercents = evaluate("Percents").toInt();
	processResponce(reading, targetPercents);
}

//interpreters::robots::details::robotParts::Sensor *WaitForLightSensorBlock::sensor() const
//{
//	return mRobotModel->lightSensor(mPort);
//}

QString WaitForLightSensorBlock::name() const
{
	return tr("Light sensor");
}
