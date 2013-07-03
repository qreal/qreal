#include "graphicalDifference.h"

using namespace versioning::details;

GraphicalDifference::GraphicalDifference(qReal::models::Models *oldModel
		, qReal::models::Models *newModel, qReal::Id const &graphicalId)
	: CommonDifference(oldModel, newModel, graphicalId)
{
	findCommonDifference();
}

qrRepo::CommonRepoApi const &GraphicalDifference::repoApi(bool const old) const
{
	return old ? mOldModel->graphicalRepoApi() : mNewModel->graphicalRepoApi();
}
