#include "dummyBlocksFactory.h"

using namespace qrTest::robotsTests::interpreterCoreTests;

using namespace qReal;

void DummyBlockFactory::setParser(interpreterBase::blocks::BlockParserInterface * const parser)
{
}

interpreterBase::blocks::BlockInterface *DummyBlockFactory::block(Id const &element)
{
	return nullptr;
}

IdList DummyBlockFactory::providedBlocks() const
{
	return IdList();
}
