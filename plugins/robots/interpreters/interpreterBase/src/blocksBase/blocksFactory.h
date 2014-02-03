#pragma once

#include <qrkernel/ids.h>

#include <interpreterBase/blocksBase/block.h>
#include <interpreterBase/blocksBase/blockParserInterface.h>
#include <interpreterBase/blocksBase/blocksTableInterface.h>
#include <interpreterBase/blocksBase/blocksFactoryInterface.h>
#include <interpreterBase/robotModel/robotModelManagerInterface.h>

namespace interpreterBase {
namespace blocksBase {

class BlocksFactory : public BlocksFactoryInterface
{
public:
	BlocksFactory(qReal::GraphicalModelAssistInterface const &graphicalModelApi
			, qReal::LogicalModelAssistInterface const &logicalModelApi
			, robotModel::RobotModelManagerInterface const &robotModelManager
			, qReal::ErrorReporterInterface * const errorReporter
			);

	BlockInterface *block(qReal::Id const &element) override;

	void setParser(BlockParserInterface * const parser) override;

//	RobotsBlockParser * getParser();

	qReal::IdList providedBlocks() const override;

private:
	static bool elementMetatypeIs(qReal::Id const &element, QString const &metatype);
	static qReal::Id id(QString const &metatype);

	robotModel::RobotModelManagerInterface const &mRobotModelManager;
	qReal::GraphicalModelAssistInterface const &mGraphicalModelApi;
	qReal::LogicalModelAssistInterface const &mLogicalModelApi;
	qReal::ErrorReporterInterface * const mErrorReporter;  // Doesn't have ownership
	BlockParserInterface *mParser;
};

}
}
