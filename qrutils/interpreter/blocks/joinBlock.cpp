#include "joinBlock.h"

#include <qrutils/interpreter/thread.h>

using namespace qReal::interpretation::blocks;

JoinBlock::JoinBlock()
	: mIncomingTokens(0)
{
}

void JoinBlock::run()
{
	const Id link = mGraphicalModelApi->graphicalRepoApi().outgoingLinks(id())[0];
	const QString survivingId = mLogicalModelApi->propertyByRoleName(mGraphicalModelApi->logicalId(link), "Guard")
			.toString();
	if (survivingId.isEmpty()) {
		error(tr("Link outgoing from join block must have surviving thread id in its 'Guard' property"));
		return;
	}

	if (mThread->id() != survivingId) {
		disconnect(SIGNAL(done()), mThread);
	}

	mIncomingTokens++;
	if (mIncomingTokens == mGraphicalModelApi->graphicalRepoApi().incomingLinks(id()).size()) {
		emit done(mNextBlockId);
	}
}
