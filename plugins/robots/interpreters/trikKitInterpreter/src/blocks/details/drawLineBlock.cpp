#include "drawLineBlock.h"

#include "src/robotModel/parts/trikDisplay.h"

using namespace trikKitInterpreter::blocks::details;

DrawLineBlock::DrawLineBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: interpreterBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void DrawLineBlock::doJob(interpreterBase::robotModel::robotParts::Display &display)
{
	auto trikDisplay = static_cast<robotModel::parts::TrikDisplay *>(&display);
	int const x1 = eval<int>("X1CoordinateLine");
	int const y1 = eval<int>("Y1CoordinateLine");
	int const x2 = eval<int>("X2CoordinateLine");
	int const y2 = eval<int>("Y2CoordinateLine");
	if (!wereParserErrors()) {
		trikDisplay->drawLine(x1, y1, x2, y2);
		emit done(mNextBlockId);
	}
}
