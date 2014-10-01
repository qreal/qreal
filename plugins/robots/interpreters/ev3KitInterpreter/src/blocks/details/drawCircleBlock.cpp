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
	int const x = eval<int>("XCoordinateCircle");
	int const y = eval<int>("YCoordinateCircle");
	int const radius = eval<int>("CircleRadius");
	if (!errorsOccured()) {
        ev3Display->drawCircle(x, y, radius);
		emit done(mNextBlockId);
	}
}
