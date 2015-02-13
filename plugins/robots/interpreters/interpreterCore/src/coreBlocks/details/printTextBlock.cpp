#include "printTextBlock.h"

#include <interpreterBase/robotModel/robotModelUtils.h>
#include <interpreterBase/robotModel/robotParts/display.h>

using namespace interpreterCore::coreBlocks::details;

PrintTextBlock::PrintTextBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: interpreterBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void PrintTextBlock::doJob(interpreterBase::robotModel::robotParts::Display &display)
{
	const int x = eval<int>("XCoordinateText");
	const int y = eval<int>("YCoordinateText");
	const QString result = eval<QString>("PrintText");
	if (!errorsOccured()) {
		display.printText(x, y, result);
		emit done(mNextBlockId);
	}
}
