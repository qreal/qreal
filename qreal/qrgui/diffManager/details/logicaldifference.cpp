#include "logicalDifference.h"

using namespace qReal::diffManager::details;

LogicalDifference::LogicalDifference(models::Models *oldModel, models::Models *newModel, qReal::Id const &logicalId)
	: CommonDifference(oldModel, newModel, logicalId)
{
	findCommonDifference();
}

qrRepo::CommonRepoApi const &LogicalDifference::repoApi(bool old) const
{
	return (old) ? mOldModel->logicalRepoApi() : mNewModel->logicalRepoApi();
}
