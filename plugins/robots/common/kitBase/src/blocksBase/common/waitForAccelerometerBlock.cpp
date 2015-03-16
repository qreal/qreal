#include "kitBase/blocksBase/common/waitForAccelerometerBlock.h"

#include "kitBase/robotModel/robotParts/accelerometerSensor.h"

using namespace kitBase;
using namespace blocksBase::common;
using namespace robotModel;

WaitForAccelerometerSensorBlock::WaitForAccelerometerSensorBlock(RobotModelInterface &robotModel)
	: WaitForSensorBlock(robotModel)
{
}

void WaitForAccelerometerSensorBlock::responseSlot(int reading)
{
	int result = eval<int>("Acceleration");
	if (!errorsOccured()) {
		processResponce(reading, result);
	}
}

DeviceInfo WaitForAccelerometerSensorBlock::device() const
{
	return DeviceInfo::create<robotParts::AccelerometerSensor>();
}
