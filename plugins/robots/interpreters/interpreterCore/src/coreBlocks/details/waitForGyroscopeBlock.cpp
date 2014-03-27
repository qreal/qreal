#include "waitforGyroscopeBlock.h"

using namespace interpreterCore::coreBlocks::details;

WaitForGyroscopeSensorBlock::WaitForGyroscopeSensorBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: WaitForSensorBlock(robotModel)
{
}

WaitForGyroscopeSensorBlock::~WaitForGyroscopeSensorBlock()
{
}

void WaitForGyroscopeSensorBlock::responseSlot(int reading)
{
	int const targetDegrees = evaluate("Degrees").toInt();
	processResponce(reading, targetDegrees);
}

//interpreters::robots::details::robotParts::Sensor *WaitForGyroscopeSensorBlock::sensor() const
//{
//	return mRobotModel->gyroscopeSensor(mPort);
//}

QString WaitForGyroscopeSensorBlock::name() const
{
	return tr("Gyroscope");
}
