#include "setPainterColorBlock.h"

#include "src/robotModel/parts/trikDisplay.h"

using namespace trikKitInterpreter::blocks::details;

SetPainterColorBlock::SetPainterColorBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
: interpreterBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void SetPainterColorBlock::doJob(interpreterBase::robotModel::robotParts::Display &display)
{
	auto trikDisplay = static_cast<robotModel::parts::TrikDisplay *>(&display);
	trikDisplay->setPainterColor(propertyToColor(stringProperty("Color")));
	emit done(mNextBlockId);
}
