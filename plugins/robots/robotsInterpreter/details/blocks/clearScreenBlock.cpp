#include "clearScreenBlock.h"
#include "../robotParts/robotModel.h"

using namespace qReal;
using namespace interpreters::robots;
using namespace qReal::interpreters::robots::details::blocks;

ClearScreenBlock::ClearScreenBlock(robotParts::Display &display)
	: mDisplay(display)
{
}

void ClearScreenBlock::run()
{
	mDisplay.clearScreen();
	emit done(mNextBlock);
}

QString ClearScreenBlock::name() const
{
	return tr("Display");
}
