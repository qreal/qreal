#pragma once

#include "interpreterBase/blocksBase/block.h"

namespace interpreterBase {
namespace blocksBase {
namespace common {

/// A block that does nothing. Used when block is not supported, but programs with it shall run correctly (for example,
/// 2d model).
class ROBOTS_INTERPRETER_BASE_EXPORT EmptyBlock : public Block
{
	Q_OBJECT

public:
	void run() override;
};

}
}
}
