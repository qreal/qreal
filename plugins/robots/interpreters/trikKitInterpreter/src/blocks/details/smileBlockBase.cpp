#include "smileBlockBase.h"

#include <interpreterBase/robotModel/robotModelUtils.h>

#include "src/robotModel/parts/trikDisplay.h"

using namespace trikKitInterpreter::blocks::details;

SmileBlockBase::SmileBlockBase(interpreterBase::robotModel::RobotModelInterface &robotModel, bool isSmileHappy)
	: mRobotModel(robotModel)
	, mIsSmileHappy(isSmileHappy)
{
}

void SmileBlockBase::run()
{
	robotModel::parts::TrikDisplay * const display =
			interpreterBase::robotModel::RobotModelUtils::findDevice<robotModel::parts::TrikDisplay>(
					mRobotModel, "DisplayPort"
			);

	display->drawSmile(!mIsSmileHappy);
	emit done(mNextBlockId);
}
