#include "drawLineBlock.h"

using namespace nxtKitInterpreter::blocks::details;

DrawLineBlock::DrawLineBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: DisplayBlock(robotModel)
{
}

void DrawLineBlock::doJob(robotModel::parts::NxtDisplay &display)
{
	int const x1 = evaluate("X1CoordinateLine").toInt();
	int const y1 = evaluate("Y1CoordinateLine").toInt();
	int const x2 = evaluate("X2CoordinateLine").toInt();
	int const y2 = evaluate("Y2CoordinateLine").toInt();
	display.drawLine(x1, y1, x2, y2);
	emit done(mNextBlockId);
}
