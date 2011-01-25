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

void LoopBlock::initNextBlocks()
{
	IdList const links = mGraphicalModelApi->graphicalRepoApi().outgoingLinks(id());

	if (links.count() == 2) {
		foreach (Id const linkId, links) {
			Id const targetBlockId = mGraphicalModelApi->graphicalRepoApi().otherEntityFromLink(linkId, id());
			Block *targetBlock = mBlocksTable->block(targetBlockId);
			Id const logicalLinkId = mGraphicalModelApi->logicalId(linkId);
			if (mLogicalModelApi->propertyByRoleName(logicalLinkId, "Guard").toString() == "iteration") {
				mIterationStartBlock = targetBlock;
			} else if (mLogicalModelApi->propertyByRoleName(logicalLinkId, "Guard").toString() == "") {
				mNextBlock = targetBlock;
			}
		}
	}
}

void LoopBlock::additionalInit()
{
	Id const logicalId = mGraphicalModelApi->logicalId(id());
	mIterations = mLogicalModelApi->propertyByRoleName(logicalId, "Iterations").toInt();
}
