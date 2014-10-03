#include "printTextBlock.h"

#include <utils/textExpressionProcessor.h>
#include <interpreterBase/robotModel/robotModelUtils.h>
#include <interpreterBase/robotModel/robotParts/display.h>

using namespace interpreterCore::coreBlocks::details;

PrintTextBlock::PrintTextBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: interpreterBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void PrintTextBlock::doJob(interpreterBase::robotModel::robotParts::Display &display)
{
	int const x = eval<int>("XCoordinateText");
	int const y = eval<int>("YCoordinateText");
	auto const result = eval<QString>("PrintText");
	if (!errorsOccured()) {
		display.printText(x, y, result);
		emit done(mNextBlockId);
	}
}
