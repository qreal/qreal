#pragma once

#include <qrutils/interpreter/block.h>

namespace qrTest {
namespace robotsTests {
namespace interpreterCoreTests {

class DummyBlock : public qReal::interpretation::Block
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
