#include "explosionCommand.h"

using namespace qReal::commands;

ExplosionCommand::ExplosionCommand(models::LogicalModelAssistApi &logicalApi
		, models::GraphicalModelAssistApi const * const graphicalApi
		, Id const &source, Id const &target, bool mustAdd)
	: mLogicalApi(logicalApi)
	, mGraphicalApi(graphicalApi)
	, mSource(source)
	, mTarget(target)
	, mMustAdd(mustAdd)
{
}

ExplosionCommand::~ExplosionCommand()
{
}

bool ExplosionCommand::execute()
{
	return processExplosion(mMustAdd);
}

bool ExplosionCommand::restoreState()
{
	return processExplosion(!mMustAdd);
}

bool ExplosionCommand::processExplosion(bool add)
{
	if (!mLogicalApi.isLogicalId(mSource) && mGraphicalApi) {
		mSource = mGraphicalApi->logicalId(mSource);
	}

	if (!mLogicalApi.isLogicalId(mTarget) && mGraphicalApi) {
		mTarget = mGraphicalApi->logicalId(mTarget);
	}

	if (add) {
		mLogicalApi.addExplosion(mSource, mTarget);
	} else {
		mLogicalApi.removeExplosion(mSource, mTarget);
	}

	return true;
}
