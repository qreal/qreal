#pragma once

#include "../semanticTransformationRule.h"

namespace generatorBase {
namespace semantics {

/// A base class for all transformation rules for loops
class LoopBlockRuleBase : public SemanticTransformationRule
{
public:
	LoopBlockRuleBase(SemanticTree *tree, const qReal::Id &id
			, LinkInfo const &iterationLink, LinkInfo const &nextLink);

protected:
	LinkInfo const &mIterationLink;
	LinkInfo const &mNextLink;
};

}
}
