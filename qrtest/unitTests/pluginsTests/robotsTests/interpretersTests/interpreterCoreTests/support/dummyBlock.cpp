#include "dummyBlock.h"

using namespace qrTest::robotsTests::interpreterCoreTests;

using namespace qReal;

void DummyBlock::run()
{
	emit done(mNextBlockId);
}

bool DummyBlock::initNextBlocks()
{
	IdList const links = mGraphicalModelApi->graphicalRepoApi().outgoingLinks(id());

	if (links.count() == 0) {
		mNextBlockId = Id();
	} else {
		mNextBlockId = mGraphicalModelApi->graphicalRepoApi().otherEntityFromLink(links[0], id());
	}

	return true;
}
