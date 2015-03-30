#include "drawPixelBlock.h"

#include "nxtKit/robotModel/parts/nxtDisplay.h"

using namespace nxt::blocks::details;

DrawPixelBlock::DrawPixelBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: kitBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void DrawPixelBlock::doJob(kitBase::robotModel::robotParts::Display &display)
{
	auto nxtDisplay = static_cast<robotModel::parts::NxtDisplay *>(&display);
	const int x = eval<int>("XCoordinatePix");
	const int y = eval<int>("YCoordinatePix");
	if (!errorsOccured()) {
		nxtDisplay->drawPixel(x, y);
		emit done(mNextBlockId);
	}
}
