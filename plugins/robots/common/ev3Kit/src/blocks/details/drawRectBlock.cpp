#include "drawRectBlock.h"

#include "ev3Kit/robotModel/parts/ev3Display.h"

using namespace ev3::blocks::details;

DrawRectBlock::DrawRectBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: kitBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void DrawRectBlock::doJob(kitBase::robotModel::robotParts::Display &display)
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
