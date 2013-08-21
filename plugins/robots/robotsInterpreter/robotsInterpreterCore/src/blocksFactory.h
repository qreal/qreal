#pragma once

#include "../../../../qrkernel/ids.h"

#include <robotsInterpreterCore/block.h>
#include <robotsInterpreterCore/robotModelInterface.h>
#include "blocksTable.h"
//#include "robotsBlockParser.h"

namespace robotsInterpreterCore {

class BlocksFactory
{
public:
	BlocksFactory(qReal::GraphicalModelAssistInterface const &graphicalModelApi
			, qReal::LogicalModelAssistInterface const &logicalModelApi
			, RobotModelInterface * const robotModel
			, qReal::ErrorReporterInterface * const errorReporter
			, BlocksTable * const blocksTable
//			, RobotsBlockParser * const parser
			);

	Block *block(qReal::Id const &element);

//	RobotsBlockParser * getParser();
private:
	RobotModelInterface * mRobotModel;  // Doesnt't have ownership
	qReal::GraphicalModelAssistInterface const &mGraphicalModelApi;
	qReal::LogicalModelAssistInterface const &mLogicalModelApi;
	qReal::ErrorReporterInterface * const mErrorReporter;  // Doesn't have ownership
	BlocksTable * const mBlocksTable;
//	RobotsBlockParser * mParser;

	static bool elementMetatypeIs(qReal::Id const &element, QString const &metatype);
};

}
