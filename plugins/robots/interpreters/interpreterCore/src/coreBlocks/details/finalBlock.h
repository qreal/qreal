#pragma once

#include <interpreterBase/blocksBase/block.h>

namespace interpreterCore {
namespace coreBlocks {
namespace details {

/// Interpreter`s implementation for final block. Stops current intepretation thread execution.
class FinalBlock : public interpreterBase::blocksBase::Block
{
	Q_OBJECT

public:
	void run() override;

private:
	bool initNextBlocks() override;
};

}
}
}
