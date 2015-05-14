#include "killThreadBlock.h"

using namespace qReal::interpretation::blocks;

void KillThreadBlock::run()
{
	const Id logicalId = mGraphicalModelApi->logicalId(id());
	const QString thread = mLogicalModelApi->propertyByRoleName(logicalId, "Thread").toString();
	if (thread.isEmpty()) {
		error(tr("Need to specify a thread to be stopped"));
	}

	emit killThread(thread);
	emit done(mNextBlockId);
}
