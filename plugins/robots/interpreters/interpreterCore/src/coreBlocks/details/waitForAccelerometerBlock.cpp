#include "waitForAccelerometerBlock.h"

#include <interpreterBase/robotModel/robotParts/accelerometerSensor.h>

using namespace interpreterCore::coreBlocks::details;
using namespace interpreterBase::robotModel;

WaitForAccelerometerSensorBlock::WaitForAccelerometerSensorBlock(RobotModelInterface &robotModel)
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

DeviceInfo WaitForAccelerometerSensorBlock::device() const
{
	return DeviceInfo::create<robotParts::AccelerometerSensor>();
}
