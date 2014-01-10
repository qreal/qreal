#pragma once

#include <interpreterBase/blocks/blocksFactoryInterface.h>

namespace qrTest {
namespace robotsTests {
namespace interpreterCoreTests {

class DummyBlockFactory : public interpreterBase::blocks::BlocksFactoryInterface
{
public:
	void setParser(interpreterBase::blocks::BlockParserInterface * const parser);
	interpreterBase::blocks::BlockInterface *block(qReal::Id const &element);
	qReal::IdList providedBlocks() const;
};

}
}
}
