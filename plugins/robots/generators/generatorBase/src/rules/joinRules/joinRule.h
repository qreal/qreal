#pragma once

#include <src/rules/semanticTransformationRule.h>

namespace generatorBase {
namespace semantics {

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
