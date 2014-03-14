#pragma once

#include "blocksFactoryInterface.h"
#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace blocksBase {

class Block;

/// Base implementation of all blocks factories. Provides common functionality and default implementations for
/// BlocksFactoryInterface methods.
class ROBOTS_INTERPRETER_BASE_EXPORT CommonBlocksFactory : public BlocksFactoryInterface
{
public:
	CommonBlocksFactory();

	void setParser(BlockParserInterface * const parser) override;
	BlockInterface *block(qReal::Id const &element) override;

	////	RobotsBlockParser * getParser();

	void configure(qReal::GraphicalModelAssistInterface const &graphicalModelApi
			, qReal::LogicalModelAssistInterface const &logicalModelApi
			, interpreterBase::robotModel::RobotModelManagerInterface &robotModelManager
			, qReal::ErrorReporterInterface &errorReporter) override;

protected:
	static qReal::Id id(QString const &metatype);
	static bool elementMetatypeIs(qReal::Id const &element, QString const &metatype);

	/// Implementation must instantiate a concrete block object by the given id. Future preparation
	/// is performed in parent code.
	virtual Block *produceBlock(qReal::Id const &element) = 0;

	interpreterBase::robotModel::RobotModelManagerInterface *mRobotModelManager;
	qReal::GraphicalModelAssistInterface const *mGraphicalModelApi;
	qReal::LogicalModelAssistInterface const *mLogicalModelApi;
	qReal::ErrorReporterInterface *mErrorReporter;
	interpreterBase::blocksBase::BlockParserInterface *mParser;
};

}
}
