#include "systemCommandBlock.h"

using namespace trik::blocks::details;
using namespace kitBase::robotModel;

SystemCommandBlock::SystemCommandBlock(RobotModelInterface &robotModel)
	: kitBase::blocksBase::common::DeviceBlock<robotModel::parts::TrikShell>(robotModel)
{
}

void SystemCommandBlock::doJob(robotModel::parts::TrikShell &shell)
{
	const QString command = stringProperty("Command").replace("\"", "\\\"");
	shell.runCommand(command);

	emit done(mNextBlockId);
}
