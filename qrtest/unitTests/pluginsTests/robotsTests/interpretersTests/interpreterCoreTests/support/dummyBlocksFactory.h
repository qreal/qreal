#pragma once

#include <interpreterBase/blocksBase/commonBlocksFactory.h>

namespace qrTest {
namespace robotsTests {
namespace interpreterCoreTests {

class DummyBlockFactory : public interpreterBase::blocksBase::CommonBlocksFactory
{
public:
	interpreterBase::blocksBase::Block *produceBlock(qReal::Id const &element) override;
	qReal::IdList providedBlocks() const override;
	qReal::IdList blocksToDisable() const override;
};

}
}
}
