#pragma once

#include "simpleVisitedRuleBase.h"

namespace generatorBase {
namespace semantics {

/// This rule is applied to blocks with simple semantics when it is connected to the
/// visited node. Creates infinite cycle starting from that node.
class SimpleVisitedOneZoneRule : public SimpleVisitedRuleBase
{
public:
	SimpleVisitedOneZoneRule(SemanticTree *tree, const qReal::Id &id, LinkInfo const &next);

	virtual bool apply();
};

}
}
