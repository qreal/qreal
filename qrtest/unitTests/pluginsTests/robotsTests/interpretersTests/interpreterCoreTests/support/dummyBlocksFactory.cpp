#include "dummyBlocksFactory.h"

#include "dummyBlock.h"

using namespace qrTest::robotsTests::interpreterCoreTests;

using namespace qReal;

DummyBlockFactory::DummyBlockFactory()
	: mRobotModelManager(nullptr)
	, mGraphicalModelApi(nullptr)
	, mLogicalModelApi(nullptr)
	, mErrorReporter(nullptr)
	, mParser(nullptr)
{
}

DummyBlockFactory::DummyBlockFactory(
		qReal::GraphicalModelAssistInterface const &graphicalModelApi
		, qReal::LogicalModelAssistInterface const &logicalModelApi
		, interpreterBase::robotModel::RobotModelManagerInterface &robotModelManager
		, qReal::ErrorReporterInterface &errorReporter
		)
	: mRobotModelManager(&robotModelManager)
	, mGraphicalModelApi(&graphicalModelApi)
	, mLogicalModelApi(&logicalModelApi)
	, mErrorReporter(&errorReporter)
	, mParser(nullptr)
{
}

void DummyBlockFactory::setParser(interpreterBase::blocksBase::BlockParserInterface * const parser)
{
	mParser = parser;
}

interpreterBase::blocksBase::BlockInterface *DummyBlockFactory::block(Id const &element)
{
	Q_UNUSED(element);

	interpreterBase::blocksBase::Block *result = new DummyBlock();

	if (mGraphicalModelApi && mLogicalModelApi && mRobotModelManager) {
		result->init(element, *mGraphicalModelApi, *mLogicalModelApi, mErrorReporter, mParser, *mRobotModelManager);
	}

	return result;
}

IdList DummyBlockFactory::providedBlocks() const
{
	return IdList();
}
