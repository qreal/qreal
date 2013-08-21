#pragma once

#include "../semanticTransformationRule.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class IfBlockRuleBase : public SemanticTransformationRule
{
public:
	IfBlockRuleBase(SemanticTree *tree, Id const &id
			, LinkInfo const &thenNode, LinkInfo const &elseNode);

protected:
	LinkInfo const &mThenLink;
	LinkInfo const &mElseLink;
};

}
}
}
}
