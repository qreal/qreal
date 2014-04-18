#pragma once

#include "generatorBase/robotsGeneratorDeclSpec.h"
#include "generatorBase/controlFlowGeneratorBase.h"
#include "generatorBase/semanticTree/semanticTree.h"
#include "rules/semanticTransformationRule.h"

namespace qReal {
namespace robots {
namespace generators {

/// Generates semantic tree in readable style. Code is represented in readable by
/// human style: it consists of if-operators, loop-blocks, switches and so on.
/// If control flow can`t be represented with structured code generation process
/// is considered to be unsuccessfull.
class ROBOTS_GENERATOR_EXPORT ReadableControlFlowGenerator : public ControlFlowGeneratorBase
{
	Q_OBJECT

public:
	ReadableControlFlowGenerator(
			qrRepo::RepoApi const &repo
			, ErrorReporterInterface &errorReporter
			, GeneratorCustomizer &customizer
			, Id const &diagramId
			, QObject *parent = 0
			, bool isThisDiagramMain = true);

	/// Implementation of clone operation for readable generator
	ControlFlowGeneratorBase *cloneFor(Id const &diagramId) override;

	/// Implementation of generation process for readable generator.
	/// Important: the graph in the model would be traversed two times
	/// for the emulation of some priority for semantic rules.
	semantics::SemanticTree *generate() override;

	void beforeSearch() override;

	void visitRegular(Id const &id, QList<LinkInfo> const &links) override;
	void visitFinal(Id const &id, QList<LinkInfo> const &links) override;
	void visitConditional(Id const &id, QList<LinkInfo> const &links) override;
	void visitLoop(Id const &id, QList<LinkInfo> const &links) override;
	void visitSwitch(Id const &id, QList<LinkInfo> const &links) override;
	void visitFork(Id const &id, QList<LinkInfo> const &links) override;

	/// This method can be used for semantic tree debug printing after all
	/// traversal stages.
	void afterSearch() override;

	/// Returns true if some generation errors occured but the generation process may be proceeded with other
	/// control flow generators (non-fatal errors occured).
	bool cantBeGeneratedIntoStructuredCode() const;

private:
	bool applyFirstPossible(Id const &currentId
			, QList<semantics::SemanticTransformationRule *> const &rules
			, bool thereWillBeMoreRules);

	bool mTravelingForSecondTime;
	bool mSomethingChangedThisIteration;
	QMap<Id, bool> mAlreadyApplied;
	bool mCantBeGeneratedIntoStructuredCode;
};

}
}
}
