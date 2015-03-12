#pragma once

#include <kitBase/blocksBase/commonBlocksFactory.h>

namespace trik {
namespace blocks {

class TrikBlocksFactory : public kitBase::blocksBase::CommonBlocksFactory
{
public:
	qReal::interpretation::Block *produceBlock(const qReal::Id &element) override;
	qReal::IdList providedBlocks() const override;
	qReal::IdList blocksToDisable() const override;
};

}
}
