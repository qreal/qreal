#pragma once

#include <interpreterBase/blocksBase/block.h>

namespace interpreterCore {
namespace coreBlocks {
namespace details {

/// Interpreter`s implementation for initial block.
/// Does nothing except passing control to next block.
class InitialBlock : public interpreterBase::blocksBase::Block
{
	Q_OBJECT

public:
	virtual void run();
};

}
}
}
