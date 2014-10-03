#include "interpreterBase/blocksBase/common/waitForGyroscopeSensorBlock.h"

#include "interpreterBase/robotModel/robotParts/gyroscopeSensor.h"

using namespace interpreterBase;
using namespace blocksBase::common;
using namespace robotModel;

WaitForGyroscopeSensorBlock::WaitForGyroscopeSensorBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: WaitForSensorBlock(robotModel)
{
}

void WaitForGyroscopeSensorBlock::responseSlot(int reading)
{
	int const result = eval<int>("Degrees");
	if (!errorsOccured()) {
		processResponce(reading, result);
	}
}

interpreterBase::robotModel::DeviceInfo WaitForGyroscopeSensorBlock::device() const
{
	return DeviceInfo::create<robotParts::GyroscopeSensor>();
}
