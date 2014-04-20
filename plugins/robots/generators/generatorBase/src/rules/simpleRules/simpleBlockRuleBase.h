#pragma once

#include "../semanticTransformationRule.h"

namespace generatorBase {
namespace semantics {

/// A base class for all simple-block semantic transformation rules
class SimpleBlockRuleBase : public SemanticTransformationRule
{
public:
	SimpleBlockRuleBase(SemanticTree *tree, qReal::Id const &id, LinkInfo const &next);

protected:
	LinkInfo const &mNextLink;
};

}
}
