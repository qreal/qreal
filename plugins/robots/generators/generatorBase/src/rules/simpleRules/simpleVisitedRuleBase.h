#pragma once

#include "simpleBlockRuleBase.h"

namespace generatorBase {
namespace semantics {

/// A base for all rules applied to blocks with simple semantics who lead to a
/// visited block.
class SimpleVisitedRuleBase : public SimpleBlockRuleBase
{
public:
	SimpleVisitedRuleBase(SemanticTree *tree, qReal::Id const &id, LinkInfo const &next);

	virtual bool apply();

protected:
	NonZoneNode *mThisNode;  // Doesn`t take ownership
	NonZoneNode *mNextNode;  // Doesn`t take ownership
};

}
}
