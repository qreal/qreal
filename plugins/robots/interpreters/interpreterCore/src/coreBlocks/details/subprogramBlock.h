#pragma once

#include <interpreterBase/blocksBase/block.h>

namespace interpreterCore {
namespace coreBlocks {
namespace details {

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
