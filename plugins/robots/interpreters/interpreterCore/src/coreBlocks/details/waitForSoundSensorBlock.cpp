#include "waitForSoundSensorBlock.h"

#include <interpreterBase/robotModel/robotParts/soundSensor.h>

using namespace interpreterCore::coreBlocks::details;
using namespace interpreterBase::robotModel;

WaitForSoundSensorBlock::WaitForSoundSensorBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: WaitForSensorBlock(robotModel)
{
}

WaitForSoundSensorBlock::~WaitForSoundSensorBlock()
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
