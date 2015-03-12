#include "drawCircleBlock.h"

#include "ev3Kit/robotModel/parts/ev3Display.h"

using namespace ev3::blocks::details;

DrawCircleBlock::DrawCircleBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: kitBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void DrawCircleBlock::doJob(kitBase::robotModel::robotParts::Display &display)
{
	auto ev3Display = static_cast<robotModel::parts::Ev3Display *>(&display);
	const int x = eval<int>("XCoordinateCircle");
	const int y = eval<int>("YCoordinateCircle");
	const int radius = eval<int>("CircleRadius");
	const bool filled = boolProperty("Filled");
	if (!errorsOccured()) {
		ev3Display->drawCircle(x, y, radius, filled);
		emit done(mNextBlockId);
	}
}
