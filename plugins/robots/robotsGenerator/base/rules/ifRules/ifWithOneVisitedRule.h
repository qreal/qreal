#pragma once

#include "ifBlockRuleBase.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class IfWithOneVisitedRule : public IfBlockRuleBase
{
public:
	IfWithOneVisitedRule(SemanticTree *tree, Id const &id
			, LinkInfo const &thenLink, LinkInfo const &elseLink);

	virtual bool apply();
};

}
}
}
}
