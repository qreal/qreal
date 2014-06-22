#include "interpreterBase/blocksBase/common/waitForAccelerometerBlock.h"

#include "interpreterBase/robotModel/robotParts/accelerometerSensor.h"

using namespace interpreterBase;
using namespace blocksBase::common;
using namespace robotModel;

WaitForAccelerometerSensorBlock::WaitForAccelerometerSensorBlock(RobotModelInterface &robotModel)
	: WaitForSensorBlock(robotModel)
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
