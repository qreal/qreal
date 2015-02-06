#pragma once

#include "src/rules/semanticTransformationRule.h"

namespace generatorBase {
namespace semantics {

/// Creates fork in semantic tree: the execution will proceed by all the branches given in constructor.
class ForkRule : public SemanticTransformationRule
{
public:
	ForkRule(SemanticTree *tree, const qReal::Id &id, QList<LinkInfo> const &threads, parts::Threads &threadsStorage);

	bool apply() override;

private:
	QList<LinkInfo> const mThreads;
	parts::Threads &mThreadsStorage;
};

}
}
