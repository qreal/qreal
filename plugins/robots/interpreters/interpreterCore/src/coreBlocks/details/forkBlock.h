#pragma once

#include <interpreterBase/blocksBase/block.h>

namespace interpreterCore {
namespace coreBlocks {
namespace details {

/// Interpreter`s implementation for fork block. Creates new interpretation threads by calling newThread singal.
class ForkBlock : public interpreterBase::blocksBase::Block
{
	Q_OBJECT

public:
	void run() override;

private:
	bool initNextBlocks() override;

	qReal::IdList mThreadStartBlocks;
};

}
}
}
