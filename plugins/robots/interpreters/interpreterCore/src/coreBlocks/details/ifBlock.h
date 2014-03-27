#pragma once

#include <interpreterBase/blocksBase/block.h>

namespace interpreterCore {
namespace coreBlocks {
namespace details {

class IfBlock : public interpreterBase::blocksBase::Block
{
	Q_OBJECT

public:
	IfBlock();

	void run() override;

private:
	bool initNextBlocks() override;

	qReal::Id mElseBlockId;
};

}
}
}
