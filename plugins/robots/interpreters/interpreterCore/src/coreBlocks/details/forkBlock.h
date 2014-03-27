#pragma once

#include <interpreterBase/blocksBase/block.h>

namespace interpreterCore {
namespace coreBlocks {
namespace details {

class ForkBlock : public interpreterBase::blocksBase::Block
{
	Q_OBJECT

public:
	ForkBlock();

	void run() override;

private:
	bool initNextBlocks() override;

	qReal::IdList mThreadStartBlocks;
};

}
}
}
