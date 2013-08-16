#pragma once

#include <ids.h>
#include <logicalModelAssistInterface.h>
#include <errorReporterInterface.h>

#include "generatorCustomizer.h"
#include "deepFirstSearcher.h"

namespace qReal {
namespace robots {
namespace generators {

class PrimaryControlFlowValidator : public DeepFirstSearcher::VisitorInterface
{
public:
	PrimaryControlFlowValidator(LogicalModelAssistInterface const &model
			, ErrorReporterInterface &errorReporter
			, GeneratorCustomizer const &customizer
			, Id const &diagramId);
	virtual ~PrimaryControlFlowValidator();

	bool validate();

private:
	virtual void visit(Id const &nodeId, QList<DeepFirstSearcher::LinkInfo> const &links);

	Id findInitialNode() const;
	void error(QString const &message, Id const &id);
	bool checkForConnected(DeepFirstSearcher::LinkInfo const &link);

	void validateRegular(Id const &id, QList<DeepFirstSearcher::LinkInfo> const &links);
	void validateFinalBlock(Id const &id, QList<DeepFirstSearcher::LinkInfo> const &links);
	void validateConditional(Id const &id, QList<DeepFirstSearcher::LinkInfo> const &links);
	void validateLoop(Id const &id, QList<DeepFirstSearcher::LinkInfo> const &links);
	void validateSwitch(Id const &id, QList<DeepFirstSearcher::LinkInfo> const &links);
	void validateFork(Id const &id, QList<DeepFirstSearcher::LinkInfo> const &links);

	LogicalModelAssistInterface const &mModel;
	ErrorReporterInterface &mErrorReporter;
	GeneratorCustomizer const &mCustomizer;
	Id const mDiagram;
	DeepFirstSearcher mDfser;
	bool mErrorsOccured;
};

}
}
}
