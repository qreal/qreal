#include "simpleTestBlock.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

SimpleTestBlock::SimpleTestBlock(Id const &graphicalId
		, models::GraphicalModelAssistApi const &graphicalModelApi
		, BlocksTable &blocksTable)
	: Block(graphicalId)
	, mNextBlock(NULL)
{
	IdList const links = graphicalModelApi.graphicalRepoApi().outgoingLinks(graphicalId);

	if (links.count() > 1) {
		// TODO: use ErrorReporter here
	}

	if (links.count() == 1) {
		Id const nextBlockId = graphicalModelApi.graphicalRepoApi().otherEntityFromLink(links[0], id());
		mNextBlock = blocksTable.block(nextBlockId);
	}
}

void SimpleTestBlock::run()
{
	mTimer.setInterval(500);
	mTimer.setSingleShot(true);
	connect(&mTimer, SIGNAL(timeout()), this, SLOT(timeout()));
	mTimer.start();
}

void SimpleTestBlock::timeout()
{
	emit done(mNextBlock);
}
