#pragma once

#include <interpreterBase/blocksBase/block.h>

namespace interpreterCore {
namespace coreBlocks {
namespace details {

class LoopBlock : public interpreterBase::blocksBase::Block
{
	Q_OBJECT

public:
	LoopBlock();

	void run() override;

private:
	bool initNextBlocks() override;
	void additionalInit() override;

	qReal::Id mIterationStartBlockId;
	int mIterations;
	bool mFirstRun;
};

}
}
}
