#include "drawPixelBlock.h"

#include "ev3Kit/robotModel/parts/ev3Display.h"

using namespace ev3::blocks::details;

DrawPixelBlock::DrawPixelBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: kitBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void DrawPixelBlock::doJob(kitBase::robotModel::robotParts::Display &display)
{
	auto ev3Display = static_cast<robotModel::parts::Ev3Display *>(&display);
	const int x = eval<int>("XCoordinatePix");
	const int y = eval<int>("YCoordinatePix");
	if (!errorsOccured()) {
		ev3Display->drawPixel(x, y);
		emit done(mNextBlockId);
	}
}
