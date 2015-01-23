#pragma once

#include <interpreterBase/blocksBase/commonBlocksFactory.h>

namespace interpreterCore {
namespace coreBlocks {

/// Blocks factory implmentation for common kit plugin blocks.
/// Provides blocks from 'Algorithms' palette group.
class CoreBlocksFactory : public interpreterBase::blocksBase::CommonBlocksFactory
{
public:
	qReal::interpretation::Block *produceBlock(qReal::Id const &element) override;

	qReal::IdList providedBlocks() const override;
	qReal::IdList blocksToDisable() const override;
};

}
}
