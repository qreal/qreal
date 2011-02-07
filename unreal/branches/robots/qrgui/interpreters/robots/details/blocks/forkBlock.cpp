#include "forkBlock.h"

#include <QtCore/QDebug>

using namespace qReal;
using namespace interpreters::robots::details::blocks;

ForkBlock::ForkBlock()
{
}

void ForkBlock::run()
{
	foreach (Block *block, mThreadStartBlocks)
		emit newThread(block);
	emit done(mNextBlock);
}

bool ForkBlock::initNextBlocks()
{
	IdList const links = mGraphicalModelApi->graphicalRepoApi().outgoingLinks(id());

	foreach (Id const linkId, links) {
		Id const targetBlockId = mGraphicalModelApi->graphicalRepoApi().otherEntityFromLink(linkId, id());
		if (targetBlockId == Id()) {
			error(tr("Outgoing link is not connected"));
			return false;
		}

		Block *targetBlock = mBlocksTable->block(targetBlockId);
		if (mNextBlock == NULL) {
			mNextBlock = targetBlock;
		} else {
			mThreadStartBlocks.append(targetBlock);
		}
	}

	if (!mNextBlock ) {
		error(tr("There must be an outgoing link, use \"End\" block to finish a program"));
		return false;
	}

	if (mThreadStartBlocks.isEmpty()) {
		error(tr("There must be at least two outgoing links"));
		return false;
	}

	return true;
}
