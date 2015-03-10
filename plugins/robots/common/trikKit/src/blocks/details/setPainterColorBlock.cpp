#include "setPainterColorBlock.h"

#include "trikKit/robotModel/parts/trikDisplay.h"

using namespace trik::blocks::details;

SetPainterColorBlock::SetPainterColorBlock(kitBase::robotModel::RobotModelInterface &robotModel)
: kitBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void SetPainterColorBlock::doJob(kitBase::robotModel::robotParts::Display &display)
{
	auto trikDisplay = static_cast<robotModel::parts::TrikDisplay *>(&display);
	trikDisplay->setPainterColor(propertyToColor(stringProperty("Color")));
	emit done(mNextBlockId);
}
