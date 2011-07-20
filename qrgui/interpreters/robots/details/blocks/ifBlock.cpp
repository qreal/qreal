#include "ifBlock.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

IfBlock::IfBlock()
	: mElseBlock(NULL)
{
}

void IfBlock::run()
{
	int const expressionValue = evaluate("Condition").toInt();
	if ((mCondition == tr("less than 0") && expressionValue < 0)
			|| (mCondition == tr("greater than 0") && expressionValue > 0)
			|| (mCondition == tr("equals to 0") && expressionValue == 0))
	{
		emit done(mNextBlock);
	} else {
		emit done(mElseBlock);
	}
}

bool IfBlock::initNextBlocks()
{
	bool thenFound = false;
	bool elseFound = false;

	IdList const links = mGraphicalModelApi->graphicalRepoApi().outgoingLinks(id());

	foreach (Id const linkId, links) {
		Id const targetBlockId = mGraphicalModelApi->graphicalRepoApi().otherEntityFromLink(linkId, id());
		if (targetBlockId == Id()) {
			error(tr("Outgoing link is not connected"));
			return false;
		}

		Block *targetBlock = mBlocksTable->block(targetBlockId);
		QString const condition = stringProperty(linkId, "Guard").toLower();
		if (condition == tr("less than 0")
				|| condition == tr("greater than 0")
				|| condition == tr("equals to 0"))
		{
			if (!thenFound) {
				mNextBlock = targetBlock;
				mCondition = condition;
				thenFound = true;
			} else {
				error(tr("Two links marked with condition found"));
				return false;
			}
		} else if (condition == "") {
			if (!elseFound) {
				mElseBlock = targetBlock;
				elseFound = true;
			} else {
				error(tr("Two outgoing links to a next element found"));
				return false;
			}
		}
	}

	if (!thenFound) {
		error(tr("There must be a link with property \"Guard\" set to one of the conditions"));
		return false;
	}

	if (!elseFound) {
		error(tr("There must be a non-marked outgoing link"));
		return false;
	}

	return true;
}
