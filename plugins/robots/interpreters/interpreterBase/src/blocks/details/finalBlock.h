#pragma once

#include <interpreterBase/blocks/block.h>

namespace interpreterBase {
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
