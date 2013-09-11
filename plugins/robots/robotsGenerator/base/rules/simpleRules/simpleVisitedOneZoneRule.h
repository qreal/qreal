#pragma once

#include "simpleVisitedRuleBase.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class SimpleVisitedOneZoneRule : public SimpleVisitedRuleBase
{
public:
	SimpleVisitedOneZoneRule(SemanticTree *tree, Id const &id, LinkInfo const &next);

	virtual bool apply();
};

}
}
}
}
