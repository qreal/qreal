#include "logicalDifference.h"

using namespace versioning::details;

LogicalDifference::LogicalDifference(qReal::models::Models *oldModel
		, qReal::models::Models *newModel, qReal::Id const &logicalId)
	: CommonDifference(oldModel, newModel, logicalId)
{
	findCommonDifference();
}

qrRepo::CommonRepoApi const &LogicalDifference::repoApi(bool old) const
{
	return (old) ? mOldModel->logicalRepoApi() : mNewModel->logicalRepoApi();
}
