#include "sayBlock.h"

using namespace trikKitInterpreter::blocks::details;
using namespace interpreterBase::robotModel;

SayBlock::SayBlock(RobotModelInterface &robotModel)
	: interpreterBase::blocksBase::common::DeviceBlock<robotModel::parts::TrikShell>(robotModel)
{
}

void SayBlock::doJob(robotModel::parts::TrikShell &shell)
{
	const QString text = eval<QString>("Text");
	if (!errorsOccured()) {
		shell.say(text);
		emit done(mNextBlockId);
	}
}
