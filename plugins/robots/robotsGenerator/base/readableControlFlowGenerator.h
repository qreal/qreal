#pragma once

#include "controlFlowGeneratorBase.h"
#include "semanticTree/semanticTree.h"
#include "rules/semanticTransformationRule.h"

namespace qReal {
namespace robots {
namespace generators {

class ReadableControlFlowGenerator : public ControlFlowGeneratorBase
{
public:
	ReadableControlFlowGenerator(
			LogicalModelAssistInterface const &logicalModel
			, GraphicalModelAssistInterface const &graphicalModel
			, ErrorReporterInterface &errorReporter
			, GeneratorCustomizer &customizer
			, Id const &diagramId
			, QObject *parent = 0);

	virtual ControlFlow *generate();

	virtual void beforeSearch();
	virtual void visitRegular(Id const &id, QList<LinkInfo> const &links);
	virtual void visitFinal(Id const &id, QList<LinkInfo> const &links);
	virtual void visitConditional(Id const &id, QList<LinkInfo> const &links);
	virtual void visitLoop(Id const &id, QList<LinkInfo> const &links);
	virtual void visitSwitch(Id const &id, QList<LinkInfo> const &links);
	virtual void visitFork(Id const &id, QList<LinkInfo> const &links);
	virtual void afterSearch();

private:
	bool applyFirstPossible(Id const &currentId
			, QList<semantics::SemanticTransformationRule *> const &rules
			, bool thereWillBeMoreRules);

	semantics::SemanticTree *mSemanticTree;  // Takes ownership
	bool mTravelingForSecondTime;
	QMap<Id, bool> mAlreadyApplied;
};

}
}
}
