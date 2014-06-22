#include "interpreterBase/blocksBase/common/waitForSoundSensorBlock.h"

#include "interpreterBase/robotModel/robotParts/soundSensor.h"

using namespace interpreterBase;
using namespace blocksBase::common;
using namespace robotModel;

WaitForSoundSensorBlock::WaitForSoundSensorBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: WaitForSensorBlock(robotModel)
{
}

void WaitForSoundSensorBlock::responseSlot(int reading)
{
	int const targetPercents = evaluate("Volume").toInt();
	processResponce(reading, targetPercents);
}

interpreterBase::robotModel::DeviceInfo WaitForSoundSensorBlock::device() const
{
	return DeviceInfo::create<robotParts::SoundSensor>();
}
