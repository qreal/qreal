#pragma once

#include <QtCore/QHash>

#include "../../../../qrkernel/ids.h"
#include "../../../../qrgui/models/graphicalModelAssistApi.h"
#include "../../../../qrgui/models/logicalModelAssistApi.h"
#include "../../../../qrgui/mainwindow/errorReporter.h"

#include "robotParts/robotModel.h"

#include "robotsBlockParser.h"

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
			, RobotsBlockParser * const parser
	);
	~BlocksTable();
	blocks::Block *block(Id const &element);
	void clear();
	void addBlock(Id const &element, blocks::Block *block);
	void setFailure();
	void setIdleForBlocks();

private:
	QHash<Id, blocks::Block *> mBlocks;  // Has ownership
	BlocksFactory *mBlocksFactory;  // Has ownership
};

}
}
}
}
