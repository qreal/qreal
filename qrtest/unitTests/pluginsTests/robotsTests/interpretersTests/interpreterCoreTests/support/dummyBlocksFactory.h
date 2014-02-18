#pragma once

#include <qrgui/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include <interpreterBase/blocksBase/blocksFactoryInterface.h>
#include <interpreterBase/robotModel/robotModelManagerInterface.h>

namespace qrTest {
namespace robotsTests {
namespace interpreterCoreTests {

class DummyBlockFactory : public interpreterBase::blocksBase::BlocksFactoryInterface
{
public:
	DummyBlockFactory();

	DummyBlockFactory(qReal::GraphicalModelAssistInterface const &graphicalModelApi
			, qReal::LogicalModelAssistInterface const &logicalModelApi
			, interpreterBase::robotModel::RobotModelManagerInterface &robotModelManager
			, qReal::ErrorReporterInterface &errorReporter
			);

	void setParser(interpreterBase::blocksBase::BlockParserInterface * const parser) override;
	interpreterBase::blocksBase::BlockInterface *block(qReal::Id const &element) override;
	qReal::IdList providedBlocks() const override;

private:
	interpreterBase::robotModel::RobotModelManagerInterface * const mRobotModelManager;  // Doesn't have ownership
	qReal::GraphicalModelAssistInterface const * const mGraphicalModelApi;  // Doesn't have ownership
	qReal::LogicalModelAssistInterface const * const mLogicalModelApi;  // Doesn't have ownership
	qReal::ErrorReporterInterface * const mErrorReporter;  // Doesn't have ownership
	interpreterBase::blocksBase::BlockParserInterface *mParser;  // Doesn't have ownership
};

}
}
}
