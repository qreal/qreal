#pragma once

#include <src/rules/semanticTransformationRule.h>

namespace generatorBase {
namespace semantics {

/// Creates a join of threads in semantic tree. When called from one of joined threads it will finalize execution of
/// that thread. When called from the main thread the execution will proceed after the join is complete.
class JoinRule : public SemanticTransformationRule
{
public:
	JoinRule(SemanticTree *tree
			, const qReal::Id &id
			, const QString &threadId
			, parts::Threads &threadsStorage
			, bool fromMain);

	bool apply() override;

private:
	QString mThreadId;
	parts::Threads &mThreadsStorage;
	bool mFromMain;
};

}
}
