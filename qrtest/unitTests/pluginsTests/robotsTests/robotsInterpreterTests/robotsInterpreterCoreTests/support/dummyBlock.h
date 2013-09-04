#pragma once

#include <robotsInterpreterCore/blocks/block.h>

namespace qrTest {
namespace robotsInterpreterCoreTests {

class DummyBlock : public robotsInterpreterCore::blocks::Block
{
	Q_OBJECT

public:
	virtual void run();

private:
	virtual bool initNextBlocks();
};

}
}
