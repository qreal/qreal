#pragma once

#include <interpreterBase/blocks/block.h>

namespace qrTest {
namespace robotsTests {
namespace interpreterCoreTests {

class DummyBlock : public interpreterBase::blocks::Block
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
