#include "loopBlock.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

LoopBlock::LoopBlock()
	: mIterationStartBlock(NULL)
	, mIterations(0)
	, mFirstRun(true)
{
}

void LoopBlock::run()
{
	--mIterations;
	if (mIterations < 0) {
		mFirstRun = true;
		emit done(mNextBlock);
	} else {
		emit done(mIterationStartBlock);
	}
}

bool LoopBlock::initNextBlocks()
{
	bool iterationFound = false;
	bool nextFound = false;

	IdList const links = mGraphicalModelApi->graphicalRepoApi().outgoingLinks(id());

	QString const iterationNotFoundError = tr("There must be an outgoing link with"\
			" property <b>Guard</b> set as <b>iteration</b>. See help, chapter <i>Programming</i>");
	foreach (Id const &linkId, links) {
		Id const targetBlockId = mGraphicalModelApi->graphicalRepoApi().otherEntityFromLink(linkId, id());
		if (targetBlockId.isNull()) {
			error(tr("Outgoing link is not connected"));
			return false;
		}

		Block *targetBlock = mBlocksTable->block(targetBlockId);
		if (stringProperty(linkId, "Guard").toLower() == "iteration") {
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
				error(iterationNotFoundError);
				return false;
			}
		}
	}

	if (!iterationFound) {
		error(iterationNotFoundError);
		return false;
	}

	if (!nextFound) {
		error(tr("There must be a non-marked outgoing link"));
		return false;
	}

	if (mFirstRun) {
		additionalInit();
		mFirstRun = false;
	}

	return true;
}

void LoopBlock::additionalInit()
{
	mIterations = evaluate("Iterations").toInt();
}
