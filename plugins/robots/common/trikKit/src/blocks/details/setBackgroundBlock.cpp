#include "setBackgroundBlock.h"

#include <kitBase/robotModel/robotModelUtils.h>

#include "trikKit/robotModel/parts/trikDisplay.h"

using namespace trik::blocks::details;
using namespace kitBase::robotModel::robotParts;

SetBackgroundBlock::SetBackgroundBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: kitBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void SetBackgroundBlock::doJob(kitBase::robotModel::robotParts::Display &display)
{
	auto trikDisplay = static_cast<robotModel::parts::TrikDisplay *>(&display);
	trikDisplay->setBackground(propertyToColor(stringProperty("Color")));
	emit done(mNextBlockId);
}
