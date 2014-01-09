#include "waitForAccelerometerBlock.h"

#include "../../sensorConstants.h"

using namespace qReal;
using namespace interpreters::robots;
using namespace interpreters::robots::details::blocks;

WaitForAccelerometerSensorBlock::WaitForAccelerometerSensorBlock(details::RobotModel * const robotModel)
	: WaitForSensorBlock(robotModel, robots::enums::sensorType::accelerometer)
{
}

WaitForAccelerometerSensorBlock::~WaitForAccelerometerSensorBlock()
{
}

void WaitForAccelerometerSensorBlock::responseSlot(int reading)
{
	int const targetAcceleration = evaluate("Acceleration").toInt();
	processResponce(reading, targetAcceleration);
}

interpreters::robots::details::robotParts::Sensor *WaitForAccelerometerSensorBlock::sensor() const
{
	return mRobotModel->accelerometerSensor(mPort);
}

QString WaitForAccelerometerSensorBlock::name() const
{
	return tr("Accelerometer");
}
