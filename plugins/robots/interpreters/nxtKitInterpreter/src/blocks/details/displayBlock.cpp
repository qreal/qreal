#include "displayBlock.h"

#include <utils/tracer.h>
#include <interpreterBase/robotModel/robotModelUtils.h>

using namespace nxtKitInterpreter::blocks::details;
using namespace interpreterBase::robotModel;

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
	robotModel::parts::NxtDisplay * const display
			= RobotModelUtils::findDevice<robotModel::parts::NxtDisplay>(mRobotModel, port);
	if (display) {
		doJob(*display);
	} else {
		error(tr("Display is not configured (WTF?)"));
		return;
	}
}
