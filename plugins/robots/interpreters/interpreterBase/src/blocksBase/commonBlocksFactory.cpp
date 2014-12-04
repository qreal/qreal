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
{
}

BlockInterface *CommonBlocksFactory::block(qReal::Id const &element)
{
	interpreterBase::blocksBase::Block *newBlock = blocksToDisable().contains(element.type())
			? new common::EmptyBlock
			: produceBlock(element);

	if (!newBlock) {
		newBlock = new common::EmptyBlock;
	}

	newBlock->init(element, *mGraphicalModelApi, *mLogicalModelApi, mErrorReporter, *mRobotModelManager, *mParser);

	return newBlock;
}

void CommonBlocksFactory::configure(qReal::GraphicalModelAssistInterface const &graphicalModelApi
		, qReal::LogicalModelAssistInterface const &logicalModelApi
		, interpreterBase::robotModel::RobotModelManagerInterface &robotModelManager
		, qReal::ErrorReporterInterface &errorReporter
		, qrtext::LanguageToolboxInterface &textLanguageToolbox)
{
	mRobotModelManager = &robotModelManager;
	mGraphicalModelApi = &graphicalModelApi;
	mLogicalModelApi = &logicalModelApi;
	mErrorReporter = &errorReporter;
	mParser = &textLanguageToolbox;
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
