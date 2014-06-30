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
	int const x = evaluate("XCoordinateCircle").toInt();
	int const y = evaluate("YCoordinateCircle").toInt();
	int const radius = evaluate("CircleRadius").toInt();
	nxtDisplay->drawCircle(x, y, radius);
	emit done(mNextBlockId);
}
