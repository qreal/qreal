#include "waitforGyroscopeSensorBlock.h"

#include "../../sensorConstants.h"

using namespace qReal;
using namespace interpreters::robots;
using namespace interpreters::robots::details::blocks;

WaitForGyroscopeSensorBlock::WaitForGyroscopeSensorBlock(details::RobotModel * const robotModel)
	: WaitForSensorBlock(robotModel, robots::enums::sensorType::gyroscope)
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

interpreters::robots::details::robotParts::Sensor *WaitForGyroscopeSensorBlock::sensor() const
{
	return mRobotModel->gyroscopeSensor(mPort);
}

QString WaitForGyroscopeSensorBlock::name() const
{
	return tr("Gyroscope");
}
