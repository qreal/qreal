#include "drawCircleBlock.h"

#include "nxtKit/robotModel/parts/nxtDisplay.h"

using namespace nxt::blocks::details;

DrawCircleBlock::DrawCircleBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: kitBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void DrawCircleBlock::doJob(kitBase::robotModel::robotParts::Display &display)
{
	auto nxtDisplay = static_cast<robotModel::parts::NxtDisplay *>(&display);
	const int x = eval<int>("XCoordinateCircle");
	const int y = eval<int>("YCoordinateCircle");
	const int radius = eval<int>("CircleRadius");
	if (!errorsOccured()) {
		nxtDisplay->drawCircle(x, y, radius);
		emit done(mNextBlockId);
	}
}
