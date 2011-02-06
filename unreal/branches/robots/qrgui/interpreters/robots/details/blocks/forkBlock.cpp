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

bool ForkBlock::initNextBlocks()
{
	bool otherFound = false;
	bool nextFound = false;

	IdList const links = mGraphicalModelApi->graphicalRepoApi().outgoingLinks(id());

	foreach (Id const linkId, links) {
		Id const targetBlockId = mGraphicalModelApi->graphicalRepoApi().otherEntityFromLink(linkId, id());
		if (targetBlockId == Id()) {
			error(tr("Outgoing link is not connected"));
			return false;
		}

		Block *targetBlock = mBlocksTable->block(targetBlockId);
		if (stringProperty(linkId, "Guard").toLower() == tr("other")) {
			if (!otherFound) {
				mThreadStartBlock = targetBlock;
				otherFound = true;
			} else {
				error(tr("Two links marked as \"other\" found"));
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

	if (!otherFound) {
		error(tr("There must be a link with property \"Guard\" set as \"other\""));
		return false;
	}

	if (!nextFound) {
		error(tr("There must be a non-marked outgoing link"));
		return false;
	}

	return true;
}
