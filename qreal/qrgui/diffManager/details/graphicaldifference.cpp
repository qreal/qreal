#include "graphicaldifference.h"

using namespace qReal::diffManager::details;

GraphicalDifference::GraphicalDifference(models::Models *oldModel, models::Models *newModel, qReal::Id const &graphicalId)
	: CommonDifference(oldModel, newModel, graphicalId)
{
	findCommonDifference();
}

qrRepo::CommonRepoApi const &GraphicalDifference::repoApi(bool old) const
{
	return (old) ? mOldModel->graphicalRepoApi() : mNewModel->graphicalRepoApi();
}
