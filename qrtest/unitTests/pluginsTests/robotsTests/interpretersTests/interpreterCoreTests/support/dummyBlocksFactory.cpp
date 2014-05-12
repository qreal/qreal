#include "dummyBlocksFactory.h"

#include "dummyBlock.h"

using namespace qrTest::robotsTests::interpreterCoreTests;
using namespace qReal;

interpreterBase::blocksBase::Block *DummyBlockFactory::produceBlock(Id const &element)
{
	Q_UNUSED(element)
	return new DummyBlock;
}

IdList DummyBlockFactory::providedBlocks() const
{
	return {};
}

IdList DummyBlockFactory::blocksToDisable() const
{
	return {};
}
