#include "loopBlock.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

LoopBlock::LoopBlock()
	: mIterationStartBlock(NULL)
	, mIterations(0)
{
}

void LoopBlock::run()
{
	if (mIterations <= 0)
		emit done(mNextBlock);
	else
		emit done(mIterationStartBlock);
	--mIterations;
}

bool LoopBlock::initNextBlocks()
{
	IdList const links = mGraphicalModelApi->graphicalRepoApi().outgoingLinks(id());

	if (links.count() == 2) {
		foreach (Id const linkId, links) {
			Id const targetBlockId = mGraphicalModelApi->graphicalRepoApi().otherEntityFromLink(linkId, id());
			Block *targetBlock = mBlocksTable->block(targetBlockId);
			if (stringProperty(linkId, "Guard") == "iteration") {
				mIterationStartBlock = targetBlock;
			} else if (stringProperty(linkId, "Guard") == "") {
				mNextBlock = targetBlock;
			}
		}
	}
	return true;
}

void LoopBlock::additionalInit()
{
	mIterations = intProperty("Iterations");
}
