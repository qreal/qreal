#include "ifBlock.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

IfBlock::IfBlock()
	: mElseBlock(NULL)
{
}

void IfBlock::run()
{
	bool const expressionValue = evaluateBool("Condition");
	emit done(expressionValue ? mNextBlock : mElseBlock);
}

bool IfBlock::initNextBlocks()
{
	// In correct case exactly 2 of this 3 would be non-null
	Block *trueBlock = NULL;
	Block *falseBlock = NULL;
	Block *nonMarkedBlock = NULL;

	IdList const links = mGraphicalModelApi->graphicalRepoApi().outgoingLinks(id());
	if (links.size() != 2) {
		error(tr("There must be exactly TWO links outgoing from if block"));
		return false;
	}

	foreach (Id const &linkId, links) {
		Id const targetBlockId = mGraphicalModelApi->graphicalRepoApi().otherEntityFromLink(linkId, id());
		if (targetBlockId.isNull() || targetBlockId == Id::rootId()) {
			error(tr("Outgoing link is not connected"));
			return false;
		}

		Block * const targetBlock = mBlocksTable->block(targetBlockId);
		QString const condition = stringProperty(linkId, "Guard").toLower();
		if (condition == "true") {
			if (trueBlock) {
				error(tr("Two links marked with 'true' found"));
				return false;
			} else {
				trueBlock = targetBlock;
			}
		} else if (condition == "false") {
			if (falseBlock) {
				error(tr("Two links marked with 'false' found"));
				return false;
			} else {
				falseBlock = targetBlock;
			}
		} else {
			if (nonMarkedBlock) {
				error(tr("There must be a link with property \"Guard\" set to one of the conditions"));
				return false;
			} else {
				nonMarkedBlock = targetBlock;
			}
		}
	}

	// Now we have correctly placed links with correct guards. Determining who is who
	if (!trueBlock) {
		mNextBlock = nonMarkedBlock;
		mElseBlock = falseBlock;
	} else if (!falseBlock) {
		mNextBlock = trueBlock;
		mElseBlock = nonMarkedBlock;
	} else if (!nonMarkedBlock) {
		mNextBlock = trueBlock;
		mElseBlock = falseBlock;
	}

	return true;
}
