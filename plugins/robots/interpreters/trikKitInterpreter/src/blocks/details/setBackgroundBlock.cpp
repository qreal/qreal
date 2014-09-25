#include "setBackgroundBlock.h"

#include <QtGui/QColor>

#include <interpreterBase/robotModel/robotModelUtils.h>

#include "src/robotModel/parts/trikDisplay.h"

using namespace trikKitInterpreter::blocks::details;
using namespace interpreterBase::robotModel::robotParts;

SetBackgroundBlock::SetBackgroundBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: mRobotModel(robotModel)
{
}

void SetBackgroundBlock::run()
{
	robotModel::parts::TrikDisplay * const display =
			interpreterBase::robotModel::RobotModelUtils::findDevice<robotModel::parts::TrikDisplay>(
					mRobotModel, "DisplayPort"
			);

	display->setBackground(propertyToColor(stringProperty("Color")));
	emit done(mNextBlockId);
}
