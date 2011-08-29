#pragma once

#include "commonDifference.h"
#include "../../qrrepo/commonRepoApi.h"

namespace qReal
{
namespace diffManager
{
namespace details
{

class GraphicalDifference : public CommonDifference
{
public:
	GraphicalDifference(models::Models *oldModel, models::Models *newModel, qReal::Id const &graphicalId);

protected:
	qrRepo::CommonRepoApi const &repoApi(bool old) const;
};

}
}
}
