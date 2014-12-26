#include "drawLineBlock.h"

#include "robotModel/parts/ev3Display.h"

using namespace ev3KitInterpreter::blocks::details;

DrawLineBlock::DrawLineBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: interpreterBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void DrawLineBlock::doJob(interpreterBase::robotModel::robotParts::Display &display)
{
	auto ev3Display = static_cast<robotModel::parts::Ev3Display *>(&display);
	int const x1 = eval<int>("X1CoordinateLine");
	int const y1 = eval<int>("Y1CoordinateLine");
	int const x2 = eval<int>("X2CoordinateLine");
	int const y2 = eval<int>("Y2CoordinateLine");
	if (!errorsOccured()) {
		ev3Display->drawLine(x1, y1, x2, y2);
		emit done(mNextBlockId);
	}
}
