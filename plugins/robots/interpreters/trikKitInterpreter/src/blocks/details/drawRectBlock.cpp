#include "drawRectBlock.h"

#include "src/robotModel/parts/trikDisplay.h"

using namespace trikKitInterpreter::blocks::details;

DrawRectBlock::DrawRectBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: interpreterBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void DrawRectBlock::doJob(interpreterBase::robotModel::robotParts::Display &display)
{
	auto trikDisplay = static_cast<robotModel::parts::TrikDisplay *>(&display);
	int const x = evaluate("XCoordinateRect").toInt();
	int const y = evaluate("YCoordinateRect").toInt();
	int const width = evaluate("WidthRect").toInt();
	int const height = evaluate("HeightRect").toInt();
	trikDisplay->drawRect(x, y, width, height);
	emit done(mNextBlockId);
}
