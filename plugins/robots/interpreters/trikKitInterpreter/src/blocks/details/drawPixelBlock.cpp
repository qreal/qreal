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
	int const x = eval<int>("XCoordinatePix");
	int const y = eval<int>("YCoordinatePix");
	if (!errorsOccured()) {
		trikDisplay->drawPixel(x, y);
		emit done(mNextBlockId);
	}
}
