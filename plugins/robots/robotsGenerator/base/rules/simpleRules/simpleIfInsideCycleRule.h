#pragma once

#include "simpleVisitedRuleBase.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class SimpleIfInsideCycleRule : public SimpleVisitedRuleBase
{
public:
	SimpleIfInsideCycleRule(SemanticTree *tree, Id const &id, LinkInfo const &next);

	virtual bool apply();
};

}
}
}
}

