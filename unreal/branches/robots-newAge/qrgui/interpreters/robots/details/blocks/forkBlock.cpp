#include "forkBlock.h"

#include <QtCore/QDebug>

using namespace qReal;
using namespace interpreters::robots::details::blocks;

ForkBlock::ForkBlock()
	: mThreadStartBlock(NULL)
{
}

void ForkBlock::run()
{
	emit newThread(mThreadStartBlock);
	emit done(mNextBlock);
}

void ForkBlock::initNextBlocks()
{
	IdList const links = mGraphicalModelApi->graphicalRepoApi().outgoingLinks(id());

	if (links.count() == 2) {
		foreach (Id const linkId, links) {
			Id const targetBlockId = mGraphicalModelApi->graphicalRepoApi().otherEntityFromLink(linkId, id());
			Block *targetBlock = mBlocksTable->block(targetBlockId);
			if (stringProperty(linkId, "Guard") == "other") {
				mThreadStartBlock = targetBlock;
			} else if (stringProperty(linkId, "Guard") == "") {
				mNextBlock = targetBlock;
			}
		}
	}
}
