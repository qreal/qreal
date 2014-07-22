#include "sayBlock.h"

#include <interpreterBase/blocksBase/common/displayBlock.h>
#include <interpreterBase/robotModel/robotModelUtils.h>

#include "src/robotModel/twoD/parts/twoDLed.h"
#include "src/robotModel/parts/trikShell.h"

using namespace trikKitInterpreter;
using namespace blocks;
using namespace details;
using namespace interpreterBase::robotModel;
using namespace utils;

SayBlock::SayBlock(RobotModelInterface &robotModel)
	: mRobotModel(robotModel)
{
}

SayBlock::~SayBlock()
{
}

void SayBlock::run()
{
	QString const text = stringProperty("Text");
	QString const port = "ShellPort";
	robotModel::parts::TrikShell * const shell
			= RobotModelUtils::findDevice<robotModel::parts::TrikShell>(mRobotModel, port);

	if (shell) {
		shell->say(text);
	} else {
		error(tr("Shell is not configured (WTF?)"));
	}

	emit done(mNextBlockId);
}

