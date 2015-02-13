#include "commentBlock.h"

using namespace qReal::interpretation::blocks;

void CommentBlock::run()
{
	emit done(mNextBlockId);
}

bool CommentBlock::initNextBlocks()
{
	const qReal::IdList links = mGraphicalModelApi->graphicalRepoApi().incomingLinks(id());
	if (!links.isEmpty()) {
		error(tr("The comment block with incoming links detected!"));
		return false;
	}

	return true;
}
