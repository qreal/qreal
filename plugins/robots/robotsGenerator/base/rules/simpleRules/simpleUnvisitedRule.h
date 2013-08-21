#pragma once

#include "simpleBlockRuleBase.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class SimpleUnvisitedRule : public SimpleBlockRuleBase
{
public:
	SimpleUnvisitedRule(SemanticTree *tree, Id const &id, LinkInfo const &next);

	virtual bool apply();
};

}
}
}
}
