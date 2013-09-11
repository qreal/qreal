#pragma once

#include "simpleVisitedRuleBase.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class SimpleMergedIfBranchesRule : public SimpleVisitedRuleBase
{
public:
	SimpleMergedIfBranchesRule(SemanticTree *tree, Id const &id, LinkInfo const &next);

	virtual bool apply();
};

}
}
}
}
