#pragma once

#include <interpreterBase/blocksBase/commonBlocksFactory.h>

namespace ev3KitInterpreter {
namespace blocks {

class Ev3BlocksFactory : public interpreterBase::blocksBase::CommonBlocksFactory
{
public:
	qReal::interpretation::Block *produceBlock(qReal::Id const &element) override;
	qReal::IdList providedBlocks() const override;
	qReal::IdList blocksToDisable() const override;
};

}
}
