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
	if ((mCondition == QString::fromUtf8("истина") && expressionValue)
			|| (mCondition == QString::fromUtf8("ложь") && !expressionValue))
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

	foreach (Id const &linkId, links) {
		Id const targetBlockId = mGraphicalModelApi->graphicalRepoApi().otherEntityFromLink(linkId, id());
		if (targetBlockId.isNull()) {
			error(tr("Outgoing link is not connected"));
			return false;
		}

		Block *targetBlock = mBlocksTable->block(targetBlockId);
		QString const condition = stringProperty(linkId, "Guard").toLower();
		if (condition == QString::fromUtf8("истина")
				|| condition == QString::fromUtf8("ложь"))
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
