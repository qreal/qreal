#pragma once

#include <qrkernel/ids.h>

#include <qrgui/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

//#include <interpreterBase/blocksBase/block.h>
//#include <interpreterBase/blocksBase/blockParserInterface.h>
//#include <interpreterBase/blocksBase/blocksTableInterface.h>
#include <interpreterBase/blocksBase/blocksFactoryInterface.h>
#include <interpreterBase/robotModel/robotModelManagerInterface.h>

namespace interpreterCore {
namespace coreBlocks {

class CoreBlocksFactory : public interpreterBase::blocksBase::BlocksFactoryInterface
{
public:
	CoreBlocksFactory(qReal::GraphicalModelAssistInterface const &graphicalModelApi
			, qReal::LogicalModelAssistInterface const &logicalModelApi
			, interpreterBase::robotModel::RobotModelManagerInterface &robotModelManager
			, qReal::ErrorReporterInterface &errorReporter
			);

	interpreterBase::blocksBase::BlockInterface *block(qReal::Id const &element) override;

	void setParser(interpreterBase::blocksBase::BlockParserInterface * const parser) override;

////	RobotsBlockParser * getParser();

	qReal::IdList providedBlocks() const override;

private:
	static bool elementMetatypeIs(qReal::Id const &element, QString const &metatype);
//	static qReal::Id id(QString const &metatype);

	interpreterBase::robotModel::RobotModelManagerInterface &mRobotModelManager;
	qReal::GraphicalModelAssistInterface const &mGraphicalModelApi;
	qReal::LogicalModelAssistInterface const &mLogicalModelApi;
	qReal::ErrorReporterInterface &mErrorReporter;
	interpreterBase::blocksBase::BlockParserInterface *mParser;
};

}
}
