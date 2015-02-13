#include "forkBlock.h"

using namespace qReal::interpretation::blocks;

void ForkBlock::run()
{
	foreach (const Id &blockId, mThreadStartBlocks) {
		emit newThread(blockId);
	}

	emit done(mNextBlockId);
}

bool ForkBlock::initNextBlocks()
{
	const IdList links = mGraphicalModelApi->graphicalRepoApi().outgoingLinks(id());

	foreach (const Id &linkId, links) {
		const Id targetBlockId = mGraphicalModelApi->graphicalRepoApi().otherEntityFromLink(linkId, id());
		if (targetBlockId.isNull()) {
			error(tr("Outgoing link is not connected"));
			return false;
		}

		if (mNextBlockId.isNull()) {
			mNextBlockId = targetBlockId;
		} else {
			mThreadStartBlocks.append(targetBlockId);
		}
	}

	if (mNextBlockId.isNull()) {
		error(tr("There must be an outgoing link, use \"End\" block to finish a program"));
		return false;
	}

	if (mThreadStartBlocks.isEmpty()) {
		error(tr("There must be at least two outgoing links"));
		return false;
	}

	return true;
}
