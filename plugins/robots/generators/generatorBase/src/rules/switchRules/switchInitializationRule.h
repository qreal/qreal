#pragma once

#include "src/rules/semanticTransformationRule.h"

namespace generatorBase {
namespace semantics {

/// Performs initialization of the switch semantic construction when met it on diagram.
class SwitchInitializationRule : public SemanticTransformationRule
{
public:
	SwitchInitializationRule(SemanticTree *tree
			, qReal::Id const &id
			, QList<LinkInfo> const &branches
			, qrRepo::RepoApi const &repo);

	bool apply() override;

	void addBranch(QVariant const &value, qReal::Id const &id);

private:
	QList<LinkInfo> const mBranches;
	qrRepo::RepoApi const &mRepo;
};

}
}
