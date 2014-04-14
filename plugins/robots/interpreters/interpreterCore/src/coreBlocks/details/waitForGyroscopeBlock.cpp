#include "waitForGyroscopeBlock.h"

#include <interpreterBase/robotModel/robotParts/gyroscopeSensor.h>

using namespace interpreterCore::coreBlocks::details;
using namespace interpreterBase::robotModel;

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

interpreterBase::robotModel::DeviceInfo WaitForGyroscopeSensorBlock::device() const
{
	return DeviceInfo::create<robotParts::GyroscopeSensor>();
}
