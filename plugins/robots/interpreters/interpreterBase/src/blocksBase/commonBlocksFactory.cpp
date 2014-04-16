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

void CommonBlocksFactory::setParser(interpreterBase::blocksBase::BlockParserInterface * const parser)
{
	/// @todo ??? is this ****?
	mParser = parser;
}

//RobotsBlockParser * CommonBlocksFactory::getParser()
//{
//	return mParser;
//}

BlockInterface *CommonBlocksFactory::block(qReal::Id const &element)
{
	interpreterBase::blocksBase::Block * const newBlock = produceBlock(element);

	if (newBlock) {
		newBlock->init(element, *mGraphicalModelApi, *mLogicalModelApi, mErrorReporter, mParser, *mRobotModelManager);
	} else {
		qDebug() << "Failed to create block for" << element;
	}

	return newBlock;
}

void CommonBlocksFactory::configure(qReal::GraphicalModelAssistInterface const &graphicalModelApi
		, qReal::LogicalModelAssistInterface const &logicalModelApi
		, interpreterBase::robotModel::RobotModelManagerInterface &robotModelManager
		, qReal::ErrorReporterInterface &errorReporter)
{
	mRobotModelManager = &robotModelManager;
	mGraphicalModelApi = &graphicalModelApi;
	mLogicalModelApi = &logicalModelApi;
	mErrorReporter = &errorReporter;
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
