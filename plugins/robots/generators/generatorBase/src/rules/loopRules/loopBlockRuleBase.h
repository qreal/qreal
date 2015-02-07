#pragma once

#include "../semanticTransformationRule.h"

namespace generatorBase {
namespace semantics {

/// A base class for all transformation rules for loops
class LoopBlockRuleBase : public SemanticTransformationRule
{
public:
	LoopBlockRuleBase(SemanticTree *tree, const qReal::Id &id
			, const LinkInfo &iterationLink, const LinkInfo &nextLink);

protected:
	const LinkInfo &mIterationLink;
	const LinkInfo &mNextLink;
};

}
}
