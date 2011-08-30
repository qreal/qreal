#pragma once

#include "commonDifference.h"
#include "../../qrrepo/commonRepoApi.h"

namespace qReal
{
namespace diffManager
{
namespace details
{

class LogicalDifference : public CommonDifference
{
public:
	LogicalDifference(models::Models *oldModel, models::Models *newModel, qReal::Id const &logicalId);

protected:
	qrRepo::CommonRepoApi const &repoApi(bool old) const;
};

}
}
}
