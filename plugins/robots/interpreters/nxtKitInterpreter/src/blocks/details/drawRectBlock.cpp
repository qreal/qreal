#include "drawRectBlock.h"

#include "robotModel/parts/nxtDisplay.h"

using namespace nxtKitInterpreter::blocks::details;

DrawRectBlock::DrawRectBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: interpreterBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void DrawRectBlock::doJob(interpreterBase::robotModel::robotParts::Display &display)
{
	auto nxtDisplay = static_cast<robotModel::parts::NxtDisplay *>(&display);
	int const x = evaluate("XCoordinateRect").toInt();
	int const y = evaluate("YCoordinateRect").toInt();
	int const width = evaluate("WidthRect").toInt();
	int const height = evaluate("HeightRect").toInt();
	nxtDisplay->drawRect(x, y, width, height);
	emit done(mNextBlockId);
}
