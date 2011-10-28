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
	BlocksFactory(GraphicalModelAssistInterface const &graphicalModelApi
			, LogicalModelAssistInterface const &logicalModelApi
			, RobotModel * const robotModel
			, ErrorReporterInterface * const errorReporter
			, BlocksTable * const blocksTable
			, RobotsBlockParser * const parser);
	blocks::Block *block(Id const &element);

	RobotsBlockParser * getParser();
private:
	RobotModel * mRobotModel;  // Doesnt't have ownership
	GraphicalModelAssistInterface const &mGraphicalModelApi;
	LogicalModelAssistInterface const &mLogicalModelApi;
	ErrorReporterInterface * const mErrorReporter;  // Doesn't have ownership
	BlocksTable * const mBlocksTable;
	RobotsBlockParser * mParser;

	static bool elementMetatypeIs(Id const &element, QString const &metatype);
};

}
}
}
}
