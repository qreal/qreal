#pragma once

#include <ids.h>
#include <logicalModelAssistInterface.h>
#include <graphicalModelAssistInterface.h>
#include <errorReporterInterface.h>

#include "generatorCustomizer.h"
#include "deepFirstSearcher.h"

namespace qReal {
namespace robots {
namespace generators {

class PrimaryControlFlowValidator : public DeepFirstSearcher::VisitorInterface
{
public:
	PrimaryControlFlowValidator(LogicalModelAssistInterface const &logicalModel
			, GraphicalModelAssistInterface const &graphicalModel
			, ErrorReporterInterface &errorReporter
			, GeneratorCustomizer const &customizer
			, Id const &diagramId);
	virtual ~PrimaryControlFlowValidator();

	bool validate();

	Id initialId() const;

	QPair<Id, Id> ifBranchesFor(Id const &id) const;

private:
	virtual void visit(Id const &nodeId, QList<DeepFirstSearcher::LinkInfo> const &links);

	void findInitialNode();
	void error(QString const &message, Id const &id);
	bool checkForConnected(DeepFirstSearcher::LinkInfo const &link);

	void validateRegular(Id const &id, QList<DeepFirstSearcher::LinkInfo> const &links);
	void validateFinalBlock(Id const &id, QList<DeepFirstSearcher::LinkInfo> const &links);
	void validateConditional(Id const &id, QList<DeepFirstSearcher::LinkInfo> const &links);
	void validateLoop(Id const &id, QList<DeepFirstSearcher::LinkInfo> const &links);
	void validateSwitch(Id const &id, QList<DeepFirstSearcher::LinkInfo> const &links);
	void validateFork(Id const &id, QList<DeepFirstSearcher::LinkInfo> const &links);

	LogicalModelAssistInterface const &mLogicalModel;
	GraphicalModelAssistInterface const &mGraphicalModel;
	ErrorReporterInterface &mErrorReporter;
	GeneratorCustomizer const &mCustomizer;
	Id const mDiagram;
	DeepFirstSearcher mDfser;
	bool mErrorsOccured;

	Id mFirstId;
	QMap<Id, QPair<Id, Id> > mIfBranches;
};

}
}
}
