#include "block.h"

using namespace qReal;
using namespace interpreters::robots::details;
using namespace blocks;

Block::Block(Id const &graphicalId, models::GraphicalModelAssistApi const &graphicalModelApi
			, BlocksTable &blocksTable)
	: mNextBlock(NULL)
	, mState(idle)
	, mGraphicalId(graphicalId)
{
	connect(this, SIGNAL(done(blocks::Block*const)), this, SLOT(finishedRunning()));

	IdList const links = graphicalModelApi.graphicalRepoApi().outgoingLinks(graphicalId);

	if (links.count() > 1) {
		// TODO: use ErrorReporter here
	}

	if (links.count() == 1) {
		Id const nextBlockId = graphicalModelApi.graphicalRepoApi().otherEntityFromLink(links[0], id());
		mNextBlock = blocksTable.block(nextBlockId);
	}
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
