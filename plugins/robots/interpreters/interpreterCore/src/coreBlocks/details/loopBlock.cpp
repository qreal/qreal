#include "loopBlock.h"

using namespace qReal;
using namespace interpreterCore::coreBlocks::details;

LoopBlock::LoopBlock()
	: mIterations(0)
	, mFirstRun(true)
{
}

void LoopBlock::run()
{
	--mIterations;
	if (mIterations < 0) {
		mFirstRun = true;
		emit done(mNextBlockId);
	} else {
		emit done(mIterationStartBlockId);
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

		if (stringProperty(linkId, "Guard").toLower() == "iteration") {
			if (!iterationFound) {
				mIterationStartBlockId = targetBlockId;
				iterationFound = true;
			} else {
				error(tr("Two links marked as \"iteration\" found"));
				return false;
			}
		} else if (stringProperty(linkId, "Guard") == "") {
			if (!nextFound) {
				mNextBlockId = targetBlockId;
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
