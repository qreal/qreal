#include "dummyBlocksFactory.h"

using namespace qrTest::robotsInterpreterCoreTests;

using namespace qReal;

void DummyBlockFactory::setParser(robotsInterpreterCore::blocks::BlockParserInterface * const parser)
{
}

robotsInterpreterCore::blocks::BlockInterface *DummyBlockFactory::block(Id const &element)
{
	return nullptr;
}

IdList DummyBlockFactory::providedBlocks() const
{
	return IdList();
}
