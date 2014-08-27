#pragma once

#include "src/rules/semanticTransformationRule.h"

namespace generatorBase {
namespace semantics {

/// A base class for all simple-block semantic transformation rules
class ForkRule : public SemanticTransformationRule
{
public:
	ForkRule(SemanticTree *tree, qReal::Id const &id, QList<LinkInfo> const &threads);

	bool apply() override;

protected:
	QList<LinkInfo> const mThreads;
};

}
}
