#include "block.h"

#include <QtCore/QDebug>
#include "../robotsBlockParser.h"

using namespace qReal;
using namespace interpreters::robots::details;
using namespace blocks;

Block::Block()
	: mNextBlock(NULL)
	, mGraphicalModelApi(NULL)
	, mLogicalModelApi(NULL)
	, mBlocksTable(NULL)
	, mGraphicalId(Id())
	, mParser(NULL)
	, mState(idle)
	, mErrorReporter(NULL)
{
	connect(this, SIGNAL(done(blocks::Block*const)), this, SLOT(finishedRunning()));
}

void Block::init(Id const &graphicalId
		, GraphicalModelAssistInterface const &graphicalModelApi
		, LogicalModelAssistInterface const &logicalModelApi
		, BlocksTable &blocksTable
		, ErrorReporterInterface * const errorReporter
		, RobotsBlockParser * const parser)
{
	mGraphicalId = graphicalId;
	mGraphicalModelApi = &graphicalModelApi;
	mLogicalModelApi = &logicalModelApi;
	mBlocksTable = &blocksTable;
	mErrorReporter = errorReporter;
	mParser = parser;
	additionalInit();
}

bool Block::initNextBlocks()
{
	IdList const links = mGraphicalModelApi->graphicalRepoApi().outgoingLinks(id());

	if (links.count() > 1) {
		error(tr("Too many outgoing links"));
		return false;
	}

	if (links.count() == 0) {
		error(tr("No outgoing links, please connect this block to something or use Final Node to end program"));
		return false;
	}

	if (links.count() == 1) {
		Id const nextBlockId = mGraphicalModelApi->graphicalRepoApi().otherEntityFromLink(links[0], id());
		if (nextBlockId == Id()) {
			error(tr("Outgoing link is not connected"));
			return false;
		}
		mNextBlock = mBlocksTable->block(nextBlockId);
	}
	return true;
}

Id const Block::id() const
{
	return mGraphicalId;
}

void Block::interpret()
{
	if ((mState == running) || (mState == failed))
		return;

	mState = running;
	bool result = initNextBlocks();
	if (result) {
		run();
	}
}

void Block::setFailedStatus()
{
	mState = failed;
}

void Block::setIdleStatus()
{
	mState = idle;
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
	//taking properties thru models api probaly won't allow to use libraries
	//don't really remember why
	/*
	Id const logicalId = mGraphicalModelApi->logicalId(id);
	return mLogicalModelApi->propertyByRoleName(logicalId, propertyName);
	*/
	Id const logicalId = mGraphicalModelApi->graphicalRepoApi().logicalId(id);
	return mLogicalModelApi->logicalRepoApi().property(logicalId, propertyName);
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

void Block::error(QString const &message)
{
	mErrorReporter->addError(message, id());
	emit failure();
}

bool Block::isCall() const
{
	return mGraphicalModelApi->graphicalRepoApi().isLibAvatar(mGraphicalId);
}

blocks::Block * Block::getCallEntryPoint() const
{
	blocks::Block *entryPoint = mBlocksTable->block(mGraphicalId); //propaply only way to return *this

	if (isCall()) {
		Id entryPointId = mGraphicalModelApi->mutableGraphicalRepoApi().getLibAvatarTarget(mGraphicalId);

		if (entryPointId != Id::rootId()) {
			entryPoint = mBlocksTable->block(entryPointId);
		}
	}
	return entryPoint;
}

QList<Block::SensorPortPair> Block::usedSensors() const
{
	return QList<SensorPortPair>();
}

QVariant Block::evaluate(const QString &propertyName)
{
	int position = 0;
	QVariant value = mParser->standartBlockParseProcess(stringProperty(propertyName), position, mGraphicalId).property("Number");
	if (mParser->hasErrors()) {
		mParser->deselect();
		emit failure();
	}
	return value;
}

void Block::stopActiveTimerInBlock()
{
}
