#pragma once

#include <interpreterBase/baseBlocks/blocksFactoryInterface.h>

namespace qrTest {
namespace robotsTests {
namespace interpreterCoreTests {

class DummyBlockFactory : public interpreterBase::baseBlocks::BlocksFactoryInterface
{
public:
	void setParser(interpreterBase::baseBlocks::BlockParserInterface * const parser);
	interpreterBase::baseBlocks::BlockInterface *block(qReal::Id const &element);
	qReal::IdList providedBlocks() const;
};

}
}
}
