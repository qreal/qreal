#include "setBackgroundBlock.h"

#include <QtGui/QColor>

#include <interpreterBase/robotModel/robotModelUtils.h>

#include "src/robotModel/parts/trikDisplay.h"

using namespace trikKitInterpreter::blocks::details;
using namespace interpreterBase::robotModel::robotParts;

SetBackgroundBlock::SetBackgroundBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: interpreterBase::blocksBase::common::DisplayBlock(robotModel)
{
}

void SetBackgroundBlock::doJob(interpreterBase::robotModel::robotParts::Display &display)
{
	auto trikDisplay = static_cast<robotModel::parts::TrikDisplay *>(&display);
	trikDisplay->setBackground(propertyToColor(stringProperty("Color")));
	emit done(mNextBlockId);
}
