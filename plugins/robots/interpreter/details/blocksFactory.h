#pragma once

#include "../../../../qrkernel/ids.h"

#include "blocks/block.h"
#include "robotParts/robotModel.h"
#include "robotsBlockParser.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {

class BlocksFactory
{
public:
	BlocksFactory(models::GraphicalModelAssistApi const &graphicalModelApi
			, models::LogicalModelAssistApi const &logicalModelApi
			, RobotModel * const robotModel
			, gui::ErrorReporter * const errorReporter
			, BlocksTable * const blocksTable
			, RobotsBlockParser * const parser);
	blocks::Block *block(Id const &element);

	RobotsBlockParser * getParser();
private:
	RobotModel * mRobotModel;  // Doesnt't have ownership
	models::GraphicalModelAssistApi const &mGraphicalModelApi;
	models::LogicalModelAssistApi const &mLogicalModelApi;
	gui::ErrorReporter * const mErrorReporter;  // Doesn't have ownership
	BlocksTable * const mBlocksTable;
	RobotsBlockParser * mParser;

	static bool elementMetatypeIs(Id const &element, QString const &metatype);
};

}
}
}
}
