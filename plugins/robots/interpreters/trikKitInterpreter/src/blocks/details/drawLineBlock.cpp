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
	int const x1 = evaluate("X1CoordinateLine").toInt();
	int const y1 = evaluate("Y1CoordinateLine").toInt();
	int const x2 = evaluate("X2CoordinateLine").toInt();
	int const y2 = evaluate("Y2CoordinateLine").toInt();
	trikDisplay->drawLine(x1, y1, x2, y2);
	emit done(mNextBlockId);
}
