#pragma once

#include "commondifference.h"
#include "../../qrrepo/commonrepoapi.h"

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
