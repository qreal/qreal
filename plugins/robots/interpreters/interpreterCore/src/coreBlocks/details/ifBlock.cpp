#include "ifBlock.h"

using namespace qReal;
using namespace interpreterCore::coreBlocks::details;

void IfBlock::run()
{
	bool const expressionValue = evaluateBool("Condition");
	emit done(expressionValue ? mNextBlockId : mElseBlockId);
}

bool IfBlock::initNextBlocks()
{
	// In correct case exactly 2 of this 3 would be non-null
	Id trueBlockId;
	Id falseBlockId;
	Id nonMarkedBlockId;

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

		QString const condition = stringProperty(linkId, "Guard").toLower();
		if (condition == QString::fromUtf8("истина")) {
			if (trueBlockId.isNull()) {
				trueBlockId = targetBlockId;
			} else {
				error(tr("Two links marked with 'true' found"));
				return false;
			}
		} else if (condition == QString::fromUtf8("ложь")) {
			if (falseBlockId.isNull()) {
				falseBlockId = targetBlockId;
			} else {
				error(tr("Two links marked with 'false' found"));
				return false;
			}
		} else {
			if (nonMarkedBlockId.isNull()) {
				nonMarkedBlockId = targetBlockId;
			} else {
				error(tr("There must be a link with property \"Guard\" set to one of the conditions"));
				return false;
			}
		}
	}

	// Now we have correctly placed links with correct guards. Determining who is who
	if (trueBlockId.isNull()) {
		mNextBlockId = nonMarkedBlockId;
		mElseBlockId = falseBlockId;
	} else if (falseBlockId.isNull()) {
		mNextBlockId = trueBlockId;
		mElseBlockId = nonMarkedBlockId;
	} else if (nonMarkedBlockId.isNull()) {
		mNextBlockId = trueBlockId;
		mElseBlockId = falseBlockId;
	}

	return true;
}
