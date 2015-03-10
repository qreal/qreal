#pragma once

#include <kitBase/blocksBase/commonBlocksFactory.h>

namespace nxt {
namespace blocks {

class NxtBlocksFactory : public kitBase::blocksBase::CommonBlocksFactory
{
public:
	qReal::interpretation::Block *produceBlock(const qReal::Id &element) override;
	qReal::IdList providedBlocks() const override;
	qReal::IdList blocksToDisable() const override;
};

}
}
