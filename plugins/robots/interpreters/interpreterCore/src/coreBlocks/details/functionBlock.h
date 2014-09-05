#pragma once

#include <interpreterBase/blocksBase/block.h>

namespace interpreterCore {
namespace coreBlocks {
namespace details {

/// Interpreter`s implementation for function block.
/// Uses expression parser for immediate expression`s value computation.
class FunctionBlock : public interpreterBase::blocksBase::Block
{
	Q_OBJECT

public:
	void run() override;
};

}
}
}
