#include "switchBlock.h"

using namespace qReal;
using namespace interpreterCore::coreBlocks::details;

void SwitchBlock::run()
{
	QString const expressionValue = evaluate("Expression").toString();
	if (mBranches.contains(expressionValue)) {
		emit done(mBranches[expressionValue]);
	} else {
		emit done(mDefaultBranch);
	}
}

bool SwitchBlock::initNextBlocks()
{
	IdList const links = mGraphicalModelApi->graphicalRepoApi().outgoingLinks(id());
	if (links.size() < 2) {
		error(tr("There must be at list TWO links outgoing from switch block"));
		return false;
	}

	Id nonMarkedBlockId;
	for (Id const &linkId : links) {
		Id const targetBlockId = mGraphicalModelApi->graphicalRepoApi().otherEntityFromLink(linkId, id());
		if (targetBlockId.isNull() || targetBlockId == Id::rootId()) {
			error(tr("Outgoing link is not connected"));
			return false;
		}

		QString const condition = stringProperty(linkId, "Guard").toLower();
		if (condition.isEmpty()) {
			if (nonMarkedBlockId.isNull()) {
				nonMarkedBlockId = targetBlockId;
			} else {
				error(tr("There must be a link with empty 'Guard' property."));
				return false;
			}
		} else {
			if (mBranches.contains(condition)) {
				error(tr("Duplicate case branch: '%1'").arg(condition));
				return false;
			}

			mBranches[condition] = targetBlockId;
		}
	}

	if (nonMarkedBlockId.isNull()) {
		error(tr("There must be a link with empty 'Guard' property."));
		return false;
	}

	return true;
}
