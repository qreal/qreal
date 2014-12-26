#include "drawPixelBlock.h"

#include "robotModel/parts/ev3Display.h"

using namespace ev3KitInterpreter::blocks::details;

DrawPixelBlock::DrawPixelBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: interpreterBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void DrawPixelBlock::doJob(interpreterBase::robotModel::robotParts::Display &display)
{
	auto ev3Display = static_cast<robotModel::parts::Ev3Display *>(&display);
	int const x = eval<int>("XCoordinatePix");
	int const y = eval<int>("YCoordinatePix");
	if (!errorsOccured()) {
		ev3Display->drawPixel(x, y);
		emit done(mNextBlockId);
	}
}
