#include "joinBlock.h"

#include <qrutils/interpreter/thread.h>

using namespace qReal::interpretation::blocks;

void JoinBlock::run()
{
	emit done(mThread->id() == mSurvivingId ? mNextBlockId : Id());
}

bool JoinBlock::initNextBlocks()
{
	if (!Block::initNextBlocks()) {
		return false;
	}

	const Id link = mGraphicalModelApi->graphicalRepoApi().outgoingLinks(id())[0];
	mSurvivingId = mLogicalModelApi->propertyByRoleName(mGraphicalModelApi->logicalId(link), "Guard").toString();
	if (mSurvivingId.isEmpty()) {
		error(tr("Link outgoing from join block must have surviving thread id in its 'Guard' property"));
		return false;
	}

	return true;
}
