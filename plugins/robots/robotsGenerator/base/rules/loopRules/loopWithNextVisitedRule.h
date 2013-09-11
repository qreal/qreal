#pragma once

#include "loopBlockRuleBase.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class LoopWithNextVisitedRule : public LoopBlockRuleBase
{
public:
	LoopWithNextVisitedRule(SemanticTree *tree, Id const &id
			, LinkInfo const &iterationLink, LinkInfo const &nextLink);

	virtual bool apply();
};

}
}
}
}
