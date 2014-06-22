#pragma once

#include <interpreterBase/blocksBase/block.h>

namespace interpreterCore {
namespace coreBlocks {
namespace details {

/// Interpreter`s implementation for loop block. When running it for the first time computes
/// and remembers iterations count and executes one branch for this number of times.
class LoopBlock : public interpreterBase::blocksBase::Block
{
	Q_OBJECT

public:
	LoopBlock();

	void run() override;

private:
	bool initNextBlocks() override;

	/// Iterations count computation occures here.
	void additionalInit();

	qReal::Id mIterationStartBlockId;
	int mIterations;
	bool mFirstRun;
};

}
}
}
