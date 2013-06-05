#include "commentBlock.h"

using namespace qReal::interpreters::robots::details::blocks;

CommentBlock::CommentBlock()
{
}

void CommentBlock::run()
{
	emit done(mNextBlock);
}

bool CommentBlock::initNextBlocks()
{
	IdList const links = mGraphicalModelApi->graphicalRepoApi().incomingLinks(id());
	if (!links.isEmpty()) {
		error(tr("The comment block with incomming liks detected!"));
		return false;
	}
	return true;
}
