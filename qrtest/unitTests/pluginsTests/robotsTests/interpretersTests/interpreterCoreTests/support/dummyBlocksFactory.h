#pragma once

#include <interpreterBase/blocksBase/blocksFactoryInterface.h>

namespace qrTest {
namespace robotsTests {
namespace interpreterCoreTests {

class DummyBlockFactory : public interpreterBase::blocksBase::BlocksFactoryInterface
{
public:
	void setParser(interpreterBase::blocksBase::BlockParserInterface * const parser) override;
	interpreterBase::blocksBase::BlockInterface *block(qReal::Id const &element) override;
	qReal::IdList providedBlocks() const override;
};

}
}
}
