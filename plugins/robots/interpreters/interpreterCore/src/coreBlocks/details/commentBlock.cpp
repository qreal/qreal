#include "commentBlock.h"

using namespace interpreterCore::coreBlocks::details;

void CommentBlock::run()
{
	emit done(mNextBlockId);
}

bool CommentBlock::initNextBlocks()
{
	qReal::IdList const links = mGraphicalModelApi->graphicalRepoApi().incomingLinks(id());
	if (!links.isEmpty()) {
		error(tr("The comment block with incoming links detected!"));
		return false;
	}

	return true;
}
