#pragma once

#include <interpreterBase/baseBlocks/block.h>

namespace qrTest {
namespace robotsTests {
namespace interpreterCoreTests {

class DummyBlock : public interpreterBase::baseBlocks::Block
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
