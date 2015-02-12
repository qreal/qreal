#include "drawRectBlock.h"

#include "robotModel/parts/ev3Display.h"

using namespace ev3KitInterpreter::blocks::details;

DrawRectBlock::DrawRectBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: interpreterBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void DrawRectBlock::doJob(interpreterBase::robotModel::robotParts::Display &display)
{
	auto ev3Display = static_cast<robotModel::parts::Ev3Display *>(&display);
	const int x = eval<int>("XCoordinateRect");
	const int y = eval<int>("YCoordinateRect");
	const int width = eval<int>("WidthRect");
	const int height = eval<int>("HeightRect");
	const bool filled = boolProperty("Filled");
	if (!errorsOccured()) {
		ev3Display->drawRect(x, y, width, height, filled);
		emit done(mNextBlockId);
	}
}
