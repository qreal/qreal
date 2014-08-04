#include "interpreterBase/blocksBase/common/waitForEncoderBlock.h"

#include "interpreterBase/robotModel/robotParts/encoderSensor.h"
#include "interpreterBase/robotModel/robotModelUtils.h"

using namespace interpreterBase;
using namespace blocksBase::common;
using namespace robotModel;

WaitForEncoderBlock::WaitForEncoderBlock(RobotModelInterface &robotModel)
	: WaitForSensorBlock(robotModel)
{
}

void WaitForEncoderBlock::responseSlot(int reading)
{
	int const tachoLimit = evaluate("TachoLimit").toInt();
	processResponce(reading, tachoLimit);
}

DeviceInfo WaitForEncoderBlock::device() const
{
	return DeviceInfo::create<robotParts::EncoderSensor>();
}
