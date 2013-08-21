#pragma once

#include "ifBlockRuleBase.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class IfWithBothUnvisitedRule : public IfBlockRuleBase
{
public:
	IfWithBothUnvisitedRule(SemanticTree *tree, Id const &id
			, LinkInfo const &thenNode, LinkInfo const &elseNode);

	virtual bool apply();
};

}
}
}
}
