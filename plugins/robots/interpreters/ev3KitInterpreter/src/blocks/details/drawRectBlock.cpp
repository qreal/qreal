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
	int const x = eval<int>("XCoordinateRect");
	int const y = eval<int>("YCoordinateRect");
	int const width = eval<int>("WidthRect");
	int const height = eval<int>("HeightRect");
	if (!errorsOccured()) {
		ev3Display->drawRect(x, y, width, height);
		emit done(mNextBlockId);
	}
}
