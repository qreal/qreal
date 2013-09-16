#pragma once

#include "../semanticTransformationRule.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

/// A base class for all transformation rules for loops
class LoopBlockRuleBase : public SemanticTransformationRule
{
public:
	LoopBlockRuleBase(SemanticTree *tree, Id const &id
			, LinkInfo const &iterationLink, LinkInfo const &nextLink);

protected:
	LinkInfo const &mIterationLink;
	LinkInfo const &mNextLink;
};

}
}
}
}
