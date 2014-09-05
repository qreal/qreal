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
	int const x = eval<int>("XCoordinateRect");
	int const y = eval<int>("YCoordinateRect");
	int const width = eval<int>("WidthRect");
	int const height = eval<int>("HeightRect");
	if (!wereParserErrors()) {
		nxtDisplay->drawRect(x, y, width, height);
		emit done(mNextBlockId);
	}
}
