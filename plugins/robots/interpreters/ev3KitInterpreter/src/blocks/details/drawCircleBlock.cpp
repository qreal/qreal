#include "drawCircleBlock.h"

#include "robotModel/parts/ev3Display.h"

using namespace ev3KitInterpreter::blocks::details;

DrawCircleBlock::DrawCircleBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: interpreterBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void DrawCircleBlock::doJob(interpreterBase::robotModel::robotParts::Display &display)
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
