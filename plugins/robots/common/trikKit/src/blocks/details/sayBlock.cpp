#include "sayBlock.h"

using namespace trik::blocks::details;
using namespace kitBase::robotModel;

SayBlock::SayBlock(RobotModelInterface &robotModel)
	: kitBase::blocksBase::common::DeviceBlock<robotModel::parts::TrikShell>(robotModel)
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
