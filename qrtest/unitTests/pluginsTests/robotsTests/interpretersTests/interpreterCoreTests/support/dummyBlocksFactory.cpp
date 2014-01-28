#include "dummyBlocksFactory.h"

#include "dummyBlock.h"

using namespace qrTest::robotsTests::interpreterCoreTests;

using namespace qReal;

void DummyBlockFactory::setParser(interpreterBase::baseBlocks::BlockParserInterface * const parser)
{
	Q_UNUSED(parser);
}

interpreterBase::baseBlocks::BlockInterface *DummyBlockFactory::block(Id const &element)
{
	Q_UNUSED(element);

	return new DummyBlock();
}

IdList DummyBlockFactory::providedBlocks() const
{
	return IdList();
}
