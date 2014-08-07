#include "drawPixelBlock.h"

#include "src/robotModel/parts/trikDisplay.h"

using namespace trikKitInterpreter::blocks::details;

DrawPixelBlock::DrawPixelBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: interpreterBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void DrawPixelBlock::doJob(interpreterBase::robotModel::robotParts::Display &display)
{
	auto trikDisplay = static_cast<robotModel::parts::TrikDisplay *>(&display);
	int const x = evaluate("XCoordinatePix").toInt();
	int const y = evaluate("YCoordinatePix").toInt();
	trikDisplay->drawPixel(x, y);
	emit done(mNextBlockId);
}
