#pragma once

#include <qrkernel/ids.h>

#include <qrgui/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include <interpreterBase/blocksBase/blocksFactoryInterface.h>
#include <interpreterBase/robotModel/robotModelManagerInterface.h>

namespace nxtKitInterpreter {
namespace blocks {

class NxtBlocksFactory : public interpreterBase::blocksBase::BlocksFactoryInterface
{
public:
	NxtBlocksFactory(qReal::GraphicalModelAssistInterface const &graphicalModelApi
			, qReal::LogicalModelAssistInterface const &logicalModelApi
			, interpreterBase::robotModel::RobotModelManagerInterface &robotModelManager
			, qReal::ErrorReporterInterface &errorReporter
			);

	interpreterBase::blocksBase::BlockInterface *block(qReal::Id const &element) override;

	void setParser(interpreterBase::blocksBase::BlockParserInterface * const parser) override;

////	RobotsBlockParser * getParser();

	qReal::IdList providedBlocks() const override;

private:
//	static qReal::Id id(QString const &metatype);

	interpreterBase::robotModel::RobotModelManagerInterface &mRobotModelManager;
	qReal::GraphicalModelAssistInterface const &mGraphicalModelApi;
	qReal::LogicalModelAssistInterface const &mLogicalModelApi;
	qReal::ErrorReporterInterface &mErrorReporter;
	interpreterBase::blocksBase::BlockParserInterface *mParser;
};

}
}
