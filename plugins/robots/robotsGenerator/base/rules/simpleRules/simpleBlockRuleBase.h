#pragma once

#include "../semanticTransformationRule.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class SimpleBlockRuleBase : public SemanticTransformationRule
{
public:
	SimpleBlockRuleBase(SemanticTree *tree, Id const &id
			, LinkInfo const &next, QObject *parent = 0);

protected:
	LinkInfo const &mNext;
};

}
}
}
}
