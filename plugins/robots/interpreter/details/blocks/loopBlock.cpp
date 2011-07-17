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
	--mIterations;
	if (mIterations <= 0)
		emit done(mNextBlock);
	else
		emit done(mIterationStartBlock);
}

bool LoopBlock::initNextBlocks()
{
	bool iterationFound = false;
	bool nextFound = false;

	IdList const links = mGraphicalModelApi->graphicalRepoApi().outgoingLinks(id());

	foreach (Id const linkId, links) {
		Id const targetBlockId = mGraphicalModelApi->graphicalRepoApi().otherEntityFromLink(linkId, id());
		if (targetBlockId == Id()) {
			error(tr("Outgoing link is not connected"));
			return false;
		}

		Block *targetBlock = mBlocksTable->block(targetBlockId);
		if (stringProperty(linkId, "Guard").toLower() == tr("iteration")) {
			if (!iterationFound) {
				mIterationStartBlock = targetBlock;
				iterationFound = true;
			} else {
				error(tr("Two links marked as \"iteration\" found"));
				return false;
			}
		} else if (stringProperty(linkId, "Guard") == "") {
			if (!nextFound) {
				mNextBlock = targetBlock;
				nextFound = true;
			} else {
				error(tr("Two outgoing links to a next element found"));
				return false;
			}
		}
	}

	if (!iterationFound) {
		error(tr("There must be a link with property \"Guard\" set as \"iteration\""));
		return false;
	}

	if (!nextFound) {
		error(tr("There must be a non-marked outgoing link"));
		return false;
	}

	return true;
}

void LoopBlock::additionalInit()
{
	mIterations = evaluate("Iterations").toInt();
}
