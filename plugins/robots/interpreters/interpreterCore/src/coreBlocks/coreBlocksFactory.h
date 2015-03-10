#pragma once

#include <kitBase/blocksBase/commonBlocksFactory.h>

namespace interpreterCore {
namespace coreBlocks {

/// Blocks factory implmentation for common kit plugin blocks.
/// Provides blocks from 'Algorithms' palette group.
class CoreBlocksFactory : public kitBase::blocksBase::CommonBlocksFactory
{
public:
	qReal::interpretation::Block *produceBlock(const qReal::Id &element) override;

	qReal::IdList providedBlocks() const override;
	qReal::IdList blocksToDisable() const override;
};

}
}
