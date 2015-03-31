#include "kitBase/blocksBase/common/waitForGyroscopeSensorBlock.h"

#include "kitBase/robotModel/robotParts/gyroscopeSensor.h"

using namespace kitBase;
using namespace blocksBase::common;
using namespace robotModel;

WaitForGyroscopeSensorBlock::WaitForGyroscopeSensorBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: WaitForSensorBlock(robotModel)
{
}

void WaitForGyroscopeSensorBlock::responseSlot(int reading)
{
	const int result = eval<int>("Degrees");
	if (!errorsOccured()) {
		processResponce(reading, result);
	}
}

kitBase::robotModel::DeviceInfo WaitForGyroscopeSensorBlock::device() const
{
	return DeviceInfo::create<robotParts::GyroscopeSensor>();
}
