#pragma once

#include "commonDifference.h"

namespace versioning
{
namespace details
{

class LogicalDifference : public CommonDifference
{
public:
	LogicalDifference(qReal::models::Models *oldModel
			, qReal::models::Models *newModel, qReal::Id const &logicalId);
	virtual ~LogicalDifference() {}

protected:
	qrRepo::CommonRepoApi const &repoApi(bool const old) const;
};

}
}
