#include "waitForAccelerometerBlock.h"

using namespace interpreterCore::coreBlocks::details;

WaitForAccelerometerSensorBlock::WaitForAccelerometerSensorBlock(
		interpreterBase::robotModel::RobotModelInterface &robotModel)
	: WaitForSensorBlock(robotModel)
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

//interpreters::robots::details::robotParts::Sensor *WaitForAccelerometerSensorBlock::sensor() const
//{
//	return mRobotModel->accelerometerSensor(mPort);
//}

QString WaitForAccelerometerSensorBlock::name() const
{
	return tr("Accelerometer");
}
