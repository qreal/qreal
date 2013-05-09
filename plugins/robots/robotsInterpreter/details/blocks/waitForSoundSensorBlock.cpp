#include "waitForSoundSensorBlock.h"

#include "../../sensorConstants.h"

using namespace qReal;
using namespace interpreters::robots;
using namespace interpreters::robots::details::blocks;

WaitForSoundSensorBlock::WaitForSoundSensorBlock(details::RobotModel * const robotModel)
	: WaitForSensorBlock(robotModel, sensorType::sound)
{
}

void WaitForSoundSensorBlock::responseSlot(int reading)
{
	int const targetPercents = evaluate("Percents").toInt();
	processResponce(reading, targetPercents);
}

interpreters::robots::details::robotParts::Sensor *WaitForSoundSensorBlock::sensor() const
{
	return mRobotModel->soundSensor(mPort);
}

QString WaitForSoundSensorBlock::name() const
{
	return tr("Sound sensor");
}
