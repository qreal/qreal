#pragma once

#include "../semanticTransformationRule.h"

namespace generatorBase {
namespace semantics {

/// A base class for all if-block semantic transformation rules
class IfBlockRuleBase : public SemanticTransformationRule
{
public:
	IfBlockRuleBase(SemanticTree *tree, qReal::Id const &id
			, LinkInfo const &thenLink, LinkInfo const &elseLink);

protected:
	LinkInfo const &mThenLink;
	LinkInfo const &mElseLink;
};

}
}
