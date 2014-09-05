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
	int result = eval<int>("Acceleration");
	if (!wereParserErrors()) {
		processResponce(reading, result);
	}
}

DeviceInfo WaitForAccelerometerSensorBlock::device() const
{
	return DeviceInfo::create<robotParts::AccelerometerSensor>();
}
