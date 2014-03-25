#include "drawCircleBlock.h"

using namespace nxtKitInterpreter::blocks::details;

DrawCircleBlock::DrawCircleBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: DisplayBlock(robotModel)
{
}

void DrawCircleBlock::doJob(robotModel::parts::NxtDisplay &display)
{
	int const x = evaluate("XCoordinateCircle").toInt();
	int const y = evaluate("YCoordinateCircle").toInt();
	int const radius = evaluate("CircleRadius").toInt();
	display.drawCircle(x, y, radius);
	emit done(mNextBlockId);
}
