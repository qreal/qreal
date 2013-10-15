#pragma once

#include "../semanticTransformationRule.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

/// A base class for all simple-block semantic transformation rules
class SimpleBlockRuleBase : public SemanticTransformationRule
{
public:
	SimpleBlockRuleBase(SemanticTree *tree, Id const &id, LinkInfo const &next);

protected:
	LinkInfo const &mNextLink;
};

}
}
}
}
