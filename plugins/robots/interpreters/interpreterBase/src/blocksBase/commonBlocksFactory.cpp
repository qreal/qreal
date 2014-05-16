#include "interpreterBase/blocksBase/commonBlocksFactory.h"
#include "interpreterBase/blocksBase/block.h"

using namespace interpreterBase::blocksBase;

CommonBlocksFactory::CommonBlocksFactory()
	: mRobotModelManager(nullptr)
	, mGraphicalModelApi(nullptr)
	, mLogicalModelApi(nullptr)
	, mErrorReporter(nullptr)
//	, mBlocksTable(blocksTable)
	, mParser(nullptr)
{
}

BlockInterface *CommonBlocksFactory::block(qReal::Id const &element)
{
	interpreterBase::blocksBase::Block * const newBlock = produceBlock(element);

	if (newBlock) {
		newBlock->init(element, *mGraphicalModelApi, *mLogicalModelApi, mErrorReporter, mParser, *mRobotModelManager);
	} else {
		/// @todo: do we need this reporting?
		//qDebug() << "Failed to create block for" << element;
	}

	return newBlock;
}

void CommonBlocksFactory::configure(qReal::GraphicalModelAssistInterface const &graphicalModelApi
		, qReal::LogicalModelAssistInterface const &logicalModelApi
		, interpreterBase::robotModel::RobotModelManagerInterface &robotModelManager
		, qReal::ErrorReporterInterface &errorReporter
		, BlockParserInterface * const parser)
{
	mRobotModelManager = &robotModelManager;
	mGraphicalModelApi = &graphicalModelApi;
	mLogicalModelApi = &logicalModelApi;
	mErrorReporter = &errorReporter;
	mParser = parser;
}

qReal::IdList CommonBlocksFactory::blocksToDisable() const
{
	return {};
}

qReal::Id CommonBlocksFactory::id(QString const &metatype)
{
	return qReal::Id("RobotsMetamodel", "RobotsDiagram", metatype);
}

bool CommonBlocksFactory::elementMetatypeIs(qReal::Id const &element, QString const &metatype)
{
	return element.type() == id(metatype);
}
