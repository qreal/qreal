#include "block.h"

using namespace qReal;
using namespace interpreters::robots::details;
using namespace blocks;

Block::Block()
	: mNextBlock(NULL)
	, mGraphicalModelApi(NULL)
	, mLogicalModelApi(NULL)
	, mBlocksTable(NULL)
	, mState(idle)
	, mGraphicalId(Id())
{
	connect(this, SIGNAL(done(blocks::Block*const)), this, SLOT(finishedRunning()));
}

void Block::init(Id const &graphicalId
		, models::GraphicalModelAssistApi const &graphicalModelApi
		, models::LogicalModelAssistApi const &logicalModelApi
		, BlocksTable &blocksTable)
{
	mGraphicalId = graphicalId;
	mGraphicalModelApi = &graphicalModelApi;
	mLogicalModelApi = &logicalModelApi;
	mBlocksTable = &blocksTable;
	initNextBlocks();
	additionalInit();
}

void Block::initNextBlocks()
{
	IdList const links = mGraphicalModelApi->graphicalRepoApi().outgoingLinks(id());

	if (links.count() > 1) {
		// TODO: use ErrorReporter here
	}

	if (links.count() == 1) {
		Id const nextBlockId = mGraphicalModelApi->graphicalRepoApi().otherEntityFromLink(links[0], id());
		mNextBlock = mBlocksTable->block(nextBlockId);
	}
}

Id const Block::id() const
{
	return mGraphicalId;
}

void Block::interpret()
{
	if (mState == running)
		return;

	mState = running;
	run();
}

void Block::finishedRunning()
{
	mState = idle;
}

QVariant Block::property(QString const &propertyName) const
{
	return property(id(), propertyName);
}

QString Block::stringProperty(QString const &propertyName) const
{
	return stringProperty(id(), propertyName);
}

int Block::intProperty(QString const &propertyName) const
{
	return intProperty(id(), propertyName);
}

bool Block::boolProperty(QString const &propertyName) const
{
	return boolProperty(id(), propertyName);
}

QVariant Block::property(Id const &id, QString const &propertyName) const
{
	Id const logicalId = mGraphicalModelApi->logicalId(id);
	return mLogicalModelApi->propertyByRoleName(logicalId, propertyName);
}

QString Block::stringProperty(Id const &id, QString const &propertyName) const
{
	return property(id, propertyName).toString();
}

int Block::intProperty(Id const &id, QString const &propertyName) const
{
	return property(id, propertyName).toInt();
}

bool Block::boolProperty(Id const &id, QString const &propertyName) const
{
	return property(id, propertyName).toBool();
}
