#include "interpreterBase/blocksBase/commonBlocksFactory.h"
#include "interpreterBase/blocksBase/block.h"
#include "interpreterBase/blocksBase/common/emptyBlock.h"

using namespace interpreterBase::blocksBase;

CommonBlocksFactory::CommonBlocksFactory()
	: mRobotModelManager(nullptr)
	, mGraphicalModelApi(nullptr)
	, mLogicalModelApi(nullptr)
	, mErrorReporter(nullptr)
	, mParser(nullptr)
	, mNewParser(nullptr)
{
}

BlockInterface *CommonBlocksFactory::block(qReal::Id const &element)
{
	interpreterBase::blocksBase::Block *newBlock = produceBlock(element);

	if (!newBlock) {
		newBlock = new common::EmptyBlock;
	}

	newBlock->init(element, *mGraphicalModelApi, *mLogicalModelApi, mErrorReporter, mParser
			, *mRobotModelManager, *mNewParser);

	return newBlock;
}

void CommonBlocksFactory::configure(qReal::GraphicalModelAssistInterface const &graphicalModelApi
		, qReal::LogicalModelAssistInterface const &logicalModelApi
		, interpreterBase::robotModel::RobotModelManagerInterface &robotModelManager
		, qReal::ErrorReporterInterface &errorReporter
		, BlockParserInterface * const parser
		, textLanguageParser::TextLanguageFacade &newParser)
{
	mRobotModelManager = &robotModelManager;
	mGraphicalModelApi = &graphicalModelApi;
	mLogicalModelApi = &logicalModelApi;
	mErrorReporter = &errorReporter;
	mParser = parser;
	mNewParser = &newParser;
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
