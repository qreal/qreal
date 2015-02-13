#include "drawCircleBlock.h"

#include "robotModel/parts/nxtDisplay.h"

using namespace nxtKitInterpreter::blocks::details;

DrawCircleBlock::DrawCircleBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: interpreterBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void DrawCircleBlock::doJob(interpreterBase::robotModel::robotParts::Display &display)
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
