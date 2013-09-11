#pragma once

#include "simpleBlockRuleBase.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class SimpleVisitedRuleBase : public SimpleBlockRuleBase
{
public:
	SimpleVisitedRuleBase(SemanticTree *tree, Id const &id, LinkInfo const &next);

	virtual bool apply();

protected:
	NonZoneNode *mThisNode;  // Doesn`t take ownership
	NonZoneNode *mNextNode;  // Doesn`t take ownership
};

}
}
}
}
