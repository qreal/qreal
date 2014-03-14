#pragma once

#include <interpreterBase/blocksBase/commonBlocksFactory.h>

namespace nxtKitInterpreter {
namespace blocks {

class NxtBlocksFactory : public interpreterBase::blocksBase::CommonBlocksFactory
{
public:
	interpreterBase::blocksBase::Block *produceBlock(qReal::Id const &element) override;
	qReal::IdList providedBlocks() const override;
};

}
}
