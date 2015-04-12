#include "kitBase/blocksBase/common/waitForColorIntensityBlock.h"

using namespace kitBase;
using namespace blocksBase::common;

WaitForColorIntensityBlock::WaitForColorIntensityBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: WaitForColorSensorBlockBase(robotModel)
{
}

void WaitForColorIntensityBlock::responseSlot(int reading)
{
	const int result = eval<int>("Intensity");
	if (!errorsOccured()) {
		processResponce(reading, result);
	}
}
