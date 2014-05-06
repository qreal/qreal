#include "interpreterBase/blocksBase/common/waitForColorIntensityBlock.h"

using namespace interpreterBase;
using namespace blocksBase::common;

WaitForColorIntensityBlock::WaitForColorIntensityBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: WaitForColorSensorBlockBase(robotModel)
{
}

WaitForColorIntensityBlock::~WaitForColorIntensityBlock()
{
}

void WaitForColorIntensityBlock::responseSlot(int reading)
{
	int const targetIntensity = evaluate("Intensity").toInt();
	processResponce(reading, targetIntensity);
}
