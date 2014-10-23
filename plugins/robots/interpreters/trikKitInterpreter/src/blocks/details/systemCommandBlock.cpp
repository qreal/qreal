#include "systemCommandBlock.h"

#include <interpreterBase/blocksBase/common/displayBlock.h>
#include <interpreterBase/robotModel/robotModelUtils.h>

#include "src/robotModel/twoD/parts/twoDLed.h"
#include "src/robotModel/parts/trikShell.h"

using namespace trikKitInterpreter;
using namespace blocks;
using namespace details;
using namespace interpreterBase::robotModel;
using namespace utils;

SystemCommandBlock::SystemCommandBlock(RobotModelInterface &robotModel)
	: mRobotModel(robotModel)
{
}

SystemCommandBlock::~SystemCommandBlock()
{
}

void SystemCommandBlock::run()
{
	QString const command = stringProperty("Command").replace("\"", "\\\"");
	QString const port = "ShellPort";
	robotModel::parts::TrikShell * const shell
			= RobotModelUtils::findDevice<robotModel::parts::TrikShell>(mRobotModel, port);

	if (shell) {
		shell->runCommand(command);
	} else {
		error(tr("Shell is not configured (WTF?)"));
	}

	emit done(mNextBlockId);
}

