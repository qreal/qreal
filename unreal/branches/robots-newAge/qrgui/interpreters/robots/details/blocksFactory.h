#pragma once

#include "../../../kernel/ids.h"

#include "blocks/block.h"
#include "robotModel.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {

class BlocksFactory
{
public:
	BlocksFactory(RobotModel * const robotModel);
	blocks::Block *block(Id const &element
		, models::GraphicalModelAssistApi const &graphicalModelApi
		, models::LogicalModelAssistApi const &logicalModelApi
		, BlocksTable &blocksTable
	);
private:
	RobotModel * mRobotModel;  // Doesnt't have ownership

	static bool elementMetatypeIs(Id const &element, QString const &metatype);
};

}
}
}
}
