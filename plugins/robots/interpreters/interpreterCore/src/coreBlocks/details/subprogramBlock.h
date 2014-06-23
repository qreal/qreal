#pragma once

#include <interpreterBase/blocksBase/block.h>

namespace interpreterCore {
namespace coreBlocks {
namespace details {

/// Interpreter`s implementation for subprogram block.
/// Starts explosed diagram interpretation using stepInto(Id) signal.
class SubprogramBlock : public interpreterBase::blocksBase::Block
{
	Q_OBJECT

public:
	SubprogramBlock();

	void run() override;
	void finishedSteppingInto() override;
};

}
}
}
