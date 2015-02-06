#pragma once

#include "src/rules/semanticTransformationRule.h"

namespace generatorBase {
namespace semantics {

/// Performs initialization of the switch semantic construction when met it on diagram.
class SwitchInitializationRule : public SemanticTransformationRule
{
public:
	SwitchInitializationRule(SemanticTree *tree
			, const qReal::Id &id
			, QList<LinkInfo> const &branches
			, const qrRepo::RepoApi &repo);

	bool apply() override;

	void addBranch(const QVariant &value, const qReal::Id &id);

private:
	QList<LinkInfo> const mBranches;
	const qrRepo::RepoApi &mRepo;
};

}
}
