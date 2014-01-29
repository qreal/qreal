#pragma once

#include <interpreterBase/baseBlocks/blocksFactoryInterface.h>

namespace qrTest {
namespace robotsTests {
namespace interpreterCoreTests {

class DummyBlockFactory : public interpreterBase::baseBlocks::BlocksFactoryInterface
{
public:
	void setParser(interpreterBase::baseBlocks::BlockParserInterface * const parser) override;
	interpreterBase::baseBlocks::BlockInterface *block(qReal::Id const &element) override;
	qReal::IdList providedBlocks() const override;
};

}
}
}
