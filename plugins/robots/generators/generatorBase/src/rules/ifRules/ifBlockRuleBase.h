#pragma once

#include "../semanticTransformationRule.h"

namespace generatorBase {
namespace semantics {

/// A base class for all if-block semantic transformation rules
class IfBlockRuleBase : public SemanticTransformationRule
{
public:
	IfBlockRuleBase(SemanticTree *tree, const qReal::Id &id
			, const LinkInfo &thenLink, const LinkInfo &elseLink);

protected:
	const LinkInfo &mThenLink;
	const LinkInfo &mElseLink;
};

}
}
