#include "interpreterBase/blocksBase/common/displayBlock.h"

#include "interpreterBase/robotModel/robotParts/display.h"
#include "interpreterBase/robotModel/robotModelUtils.h"

using namespace interpreterBase;
using namespace blocksBase::common;
using namespace robotModel;

DisplayBlock::DisplayBlock(RobotModelInterface &robotModel)
	: mRobotModel(robotModel)
{
}

DisplayBlock::~DisplayBlock()
{
}

void DisplayBlock::run()
{
	QString const port = "DisplayPort";
	robotModel::robotParts::Display * const display
			= RobotModelUtils::findDevice<robotParts::Display>(mRobotModel, port);
	if (display) {
		doJob(*display);
	} else {
		error(tr("Display is not configured (WTF?)"));
	}
}
