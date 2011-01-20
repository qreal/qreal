#include "dummyBlock.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

void DummyBlock::run()
{
	mTimer.setInterval(500);
	mTimer.setSingleShot(true);
	connect(&mTimer, SIGNAL(timeout()), this, SLOT(timeout()));
	mTimer.start();
}

void DummyBlock::timeout()
{
	emit done(mNextBlock);
}
