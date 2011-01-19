#pragma once

#include "../../../kernel/ids.h"

#include "blocks/block.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {

class BlocksFactory
{
public:
	static blocks::Block *block(Id const &element
		, models::GraphicalModelAssistApi const &graphicalModelApi
		, models::LogicalModelAssistApi const &logicalModelApi
		, BlocksTable &blocksTable
	);
};

}
}
}
}
