#include "interpreterBase/blocksBase/commonBlocksFactory.h"

#include <interpreterBase/blocksBase/robotsBlock.h>
#include <qrutils/interpreter/blocks/emptyBlock.h>

using namespace interpreterBase::blocksBase;

CommonBlocksFactory::CommonBlocksFactory()
	: mRobotModelManager(nullptr)
	, mGraphicalModelApi(nullptr)
	, mLogicalModelApi(nullptr)
	, mErrorReporter(nullptr)
	, mParser(nullptr)
{
}

qReal::interpretation::BlockInterface *CommonBlocksFactory::block(qReal::Id const &element)
{
	qReal::interpretation::Block *newBlock = blocksToDisable().contains(element.type())
			? new qReal::interpretation::blocks::EmptyBlock
			: produceBlock(element);

	if (!newBlock) {
		newBlock = new qReal::interpretation::blocks::EmptyBlock;
	}

	if (RobotsBlock * const robotsBlock = dynamic_cast<RobotsBlock *>(newBlock)) {
		robotsBlock->init(element, *mGraphicalModelApi, *mLogicalModelApi
				, mErrorReporter, *mRobotModelManager, *mParser);
	} else {
		newBlock->init(element, *mGraphicalModelApi, *mLogicalModelApi, mErrorReporter, *mParser);
	}

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
