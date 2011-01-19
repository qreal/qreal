#include "block.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

Block::Block(Id const &graphicalId)
	: mState(idle)
	, mGraphicalId(graphicalId)
{
	connect(this, SIGNAL(done(blocks::Block*const)), this, SLOT(finishedRunning()));
}

Id const Block::id() const
{
	return mGraphicalId;
}

void Block::interpret()
{
	if (mState == running)
		return;

	mState = running;
	run();
}

void Block::finishedRunning()
{
	mState = idle;
}
