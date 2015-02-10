#pragma once

#include <interpreterBase/blocksBase/commonBlocksFactory.h>

namespace trikKitInterpreter {
namespace blocks {

class TrikBlocksFactory : public interpreterBase::blocksBase::CommonBlocksFactory
{
public:
	qReal::interpretation::Block *produceBlock(const qReal::Id &element) override;
	qReal::IdList providedBlocks() const override;
	qReal::IdList blocksToDisable() const override;
};

}
}
