#include "sayBlock.h"

using namespace trikKitInterpreter::blocks::details;
using namespace interpreterBase::robotModel;

SayBlock::SayBlock(RobotModelInterface &robotModel)
	: interpreterBase::blocksBase::common::DeviceBlock<robotModel::parts::TrikShell>(robotModel)
{
}

void SayBlock::doJob(robotModel::parts::TrikShell &shell)
{
	QString const text = stringProperty("Text");
	shell.say(text);

	emit done(mNextBlockId);
}
