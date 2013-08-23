#pragma once

#include <robotsInterpreterCore/blocks/block.h>

namespace robotsInterpreterCore {
namespace blocks {
namespace details {

class FinalBlock : public Block
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
