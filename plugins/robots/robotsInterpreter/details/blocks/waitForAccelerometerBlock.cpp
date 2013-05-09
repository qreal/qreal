#include "waitForAccelerometerBlock.h"

#include "../../sensorConstants.h"

using namespace qReal;
using namespace interpreters::robots;
using namespace interpreters::robots::details::blocks;

WaitForAccelerometerSensorBlock::WaitForAccelerometerSensorBlock(details::RobotModel * const robotModel)
	: WaitForSensorBlock(robotModel, sensorType::sound)
{
}

void WaitForAccelerometerSensorBlock::responseSlot(int reading)
{
	int const targetPercents = evaluate("Percents").toInt();
	processResponce(reading, targetPercents);
}

interpreters::robots::details::robotParts::Sensor *WaitForAccelerometerSensorBlock::sensor() const
{
	return mRobotModel->soundSensor(mPort);
}

QString WaitForAccelerometerSensorBlock::name() const
{
	return tr("Sound sensor");
}
