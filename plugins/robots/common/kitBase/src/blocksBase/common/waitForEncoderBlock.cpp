#include "kitBase/blocksBase/common/waitForEncoderBlock.h"

#include "kitBase/robotModel/robotParts/encoderSensor.h"
#include "kitBase/robotModel/robotModelUtils.h"

using namespace kitBase;
using namespace blocksBase::common;
using namespace robotModel;

WaitForEncoderBlock::WaitForEncoderBlock(RobotModelInterface &robotModel)
	: WaitForSensorBlock(robotModel)
{
}

void WaitForEncoderBlock::responseSlot(int reading)
{
	const int result = eval<int>("TachoLimit");
	if (!errorsOccured()) {
		processResponce(reading, result);
	}
}

DeviceInfo WaitForEncoderBlock::device() const
{
	return DeviceInfo::create<robotParts::EncoderSensor>();
}
