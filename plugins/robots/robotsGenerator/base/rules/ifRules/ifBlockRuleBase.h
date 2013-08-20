#pragma once

#include "../semanticTransformationRule.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class IfBlockRuleBase : public SemanticTransformationRule
{
public:
	IfBlockRuleBase(SemanticTree *tree, Id const &id, QObject *parent = 0);
};

}
}
}
}
