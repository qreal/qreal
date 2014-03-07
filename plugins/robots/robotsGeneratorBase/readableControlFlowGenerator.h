#pragma once

#include "robotsGeneratorDeclSpec.h"
#include "controlFlowGeneratorBase.h"
#include "semanticTree/semanticTree.h"
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
	virtual ControlFlowGeneratorBase *cloneFor(Id const &diagramId);

	/// Implementation of generation process for readable generator.
	/// Important: the graph in the model would be traversed two times
	/// for the emulation of some priority for semantic rules.
	virtual semantics::SemanticTree *generate();

	virtual void beforeSearch();
	virtual void visitRegular(Id const &id, QList<LinkInfo> const &links);
	virtual void visitFinal(Id const &id, QList<LinkInfo> const &links);
	virtual void visitConditional(Id const &id, QList<LinkInfo> const &links);
	virtual void visitLoop(Id const &id, QList<LinkInfo> const &links);
	virtual void visitSwitch(Id const &id, QList<LinkInfo> const &links);
	virtual void visitFork(Id const &id, QList<LinkInfo> const &links);

	/// This method can be used for semantic tree debug printing after all
	/// traversal stages.
	virtual void afterSearch();

private:
	bool applyFirstPossible(Id const &currentId
			, QList<semantics::SemanticTransformationRule *> const &rules
			, bool thereWillBeMoreRules);

	bool mTravelingForSecondTime;
	bool mSomethingChangedThisIteration;
	QMap<Id, bool> mAlreadyApplied;
};

}
}
}
