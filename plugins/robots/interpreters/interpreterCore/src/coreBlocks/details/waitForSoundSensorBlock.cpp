#include "waitForSoundSensorBlock.h"

using namespace interpreterCore::coreBlocks::details;

WaitForSoundSensorBlock::WaitForSoundSensorBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: WaitForSensorBlock(robotModel)
{
}

WaitForSoundSensorBlock::~WaitForSoundSensorBlock()
{
}

void WaitForSoundSensorBlock::responseSlot(int reading)
{
	int const targetPercents = evaluate("Volume").toInt();
	processResponce(reading, targetPercents);
}

//interpreters::robots::details::robotParts::Sensor *WaitForSoundSensorBlock::sensor() const
//{
//	return mRobotModel->soundSensor(mPort);
//}

QString WaitForSoundSensorBlock::name() const
{
	return tr("Sound sensor");
}
