#include "printTextBlock.h"

#include <kitBase/robotModel/robotModelUtils.h>
#include <kitBase/robotModel/robotParts/display.h>

using namespace interpreterCore::coreBlocks::details;

PrintTextBlock::PrintTextBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: kitBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void PrintTextBlock::doJob(kitBase::robotModel::robotParts::Display &display)
{
	const int x = eval<int>("XCoordinateText");
	const int y = eval<int>("YCoordinateText");
	const QString result = eval<QString>("PrintText");
	if (!errorsOccured()) {
		display.printText(x, y, result);
		emit done(mNextBlockId);
	}
}
