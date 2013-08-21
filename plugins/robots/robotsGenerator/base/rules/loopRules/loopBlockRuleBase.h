#pragma once

#include "../semanticTransformationRule.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class LoopBlockRuleBase : public SemanticTransformationRule
{
public:
	LoopBlockRuleBase(SemanticTree *tree, Id const &id
			, LinkInfo const &iterationNode, LinkInfo const &nextNode);

protected:
	LinkInfo const &mIterationLink;
	LinkInfo const &mNextLink;
};

}
}
}
}
