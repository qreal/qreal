#pragma once

#include <interpreterBase/blocksBase/block.h>

namespace qrTest {
namespace robotsTests {
namespace interpreterCoreTests {

class DummyBlock : public interpreterBase::blocksBase::Block
{
	Q_OBJECT

public:
	virtual void run();

private:
	virtual bool initNextBlocks();
};

}
}
}
