#include "drawPixelBlock.h"

using namespace nxtKitInterpreter::blocks::details;

DrawPixelBlock::DrawPixelBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: DisplayBlock(robotModel)
{
}

void DrawPixelBlock::doJob(robotModel::parts::NxtDisplay &display)
{
	int const x = evaluate("XCoordinatePix").toInt();
	int const y = evaluate("YCoordinatePix").toInt();
	display.drawPixel(x, y);
	emit done(mNextBlockId);
}
