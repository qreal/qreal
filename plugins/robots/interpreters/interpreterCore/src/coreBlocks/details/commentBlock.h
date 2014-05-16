#pragma once

#include <interpreterBase/blocksBase/block.h>

namespace interpreterCore {
namespace coreBlocks {
namespace details {

/// Interpreter`s implementation for comment block. Does nothing except verifiyng block`s links.
class CommentBlock : public interpreterBase::blocksBase::Block
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
