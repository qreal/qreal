#pragma once

#include "commonDifference.h"

namespace versioning
{
namespace details
{

class GraphicalDifference : public CommonDifference
{
public:
	GraphicalDifference(qReal::models::Models *oldModel
			, qReal::models::Models *newModel, qReal::Id const &graphicalId);
	virtual ~GraphicalDifference() {}

protected:
	qrRepo::CommonRepoApi const &repoApi(bool const old) const;
};

}
}
