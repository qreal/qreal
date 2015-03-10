#include "kitBase/blocksBase/common/waitForSoundSensorBlock.h"

#include "kitBase/robotModel/robotParts/soundSensor.h"

using namespace kitBase;
using namespace blocksBase::common;
using namespace robotModel;

WaitForSoundSensorBlock::WaitForSoundSensorBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: WaitForSensorBlock(robotModel)
{
}

void WaitForSoundSensorBlock::responseSlot(int reading)
{
	const int result = eval<int>("Volume");
	if (!errorsOccured()) {
		processResponce(reading, result);
	}
}

kitBase::robotModel::DeviceInfo WaitForSoundSensorBlock::device() const
{
	return DeviceInfo::create<robotParts::SoundSensor>();
}
