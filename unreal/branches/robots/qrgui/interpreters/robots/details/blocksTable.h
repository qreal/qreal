#pragma once

#include <QtCore/QHash>

#include "../../../kernel/ids.h"
#include "../../../models/graphicalModelAssistApi.h"
#include "../../../models/logicalModelAssistApi.h"
#include "../../../mainwindow/errorReporter.h"

#include "robotParts/robotModel.h"

#include "../../visualDebugger/blockParser.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {

namespace blocks {
class Block;
}

class BlocksFactory;

class BlocksTable
{
public:
	BlocksTable(models::GraphicalModelAssistApi const &graphicalModelApi
			, models::LogicalModelAssistApi const &logicalModelApi
			, RobotModel * const robotModel
			, gui::ErrorReporter * const errorReporter
			, BlockParser * const parser
	);
	~BlocksTable();
	blocks::Block *block(Id const &element);
	void clear();
	void addBlock(Id const &element, blocks::Block *block);

private:
	QHash<Id, blocks::Block *> mBlocks;  // Has ownership
	BlocksFactory *mBlocksFactory;  // Has ownership
};

}
}
}
}
