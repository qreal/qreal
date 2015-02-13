#pragma once

#include "simpleBlockRuleBase.h"

namespace generatorBase {
namespace semantics {

/// A base for all rules applied to blocks with simple semantics who lead to a
/// visited block.
class SimpleVisitedRuleBase : public SimpleBlockRuleBase
{
public:
	SimpleVisitedRuleBase(SemanticTree *tree, const qReal::Id &id, const LinkInfo &next);

	virtual bool apply();

protected:
	NonZoneNode *mThisNode;  // Doesn`t take ownership
	NonZoneNode *mNextNode;  // Doesn`t take ownership
};

}
}
