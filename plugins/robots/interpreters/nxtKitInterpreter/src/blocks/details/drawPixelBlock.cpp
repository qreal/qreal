#include "drawPixelBlock.h"

#include "robotModel/parts/nxtDisplay.h"

using namespace nxtKitInterpreter::blocks::details;

DrawPixelBlock::DrawPixelBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: interpreterBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void DrawPixelBlock::doJob(interpreterBase::robotModel::robotParts::Display &display)
{
	auto nxtDisplay = static_cast<robotModel::parts::NxtDisplay *>(&display);
	const int x = eval<int>("XCoordinatePix");
	const int y = eval<int>("YCoordinatePix");
	if (!errorsOccured()) {
		nxtDisplay->drawPixel(x, y);
		emit done(mNextBlockId);
	}
}
