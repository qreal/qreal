#pragma once

#include <kitBase/blocksBase/commonBlocksFactory.h>

namespace qrTest {
namespace robotsTests {
namespace interpreterCoreTests {

class DummyBlockFactory : public kitBase::blocksBase::CommonBlocksFactory
{
public:
	qReal::interpretation::Block *produceBlock(qReal::Id const &element) override;
	qReal::IdList providedBlocks() const override;
	qReal::IdList blocksToDisable() const override;
};

}
}
}
