#pragma once

#include "controlFlow.h"
#include "primaryControlFlowValidator.h"
#include "robotsDiagramVisitor.h"

namespace qReal {
namespace robots {
namespace generators {

class ControlFlowGeneratorBase : public QObject, public RobotsDiagramVisitor
{
public:
	ControlFlowGeneratorBase(
			LogicalModelAssistInterface const &logicalModel
			, GraphicalModelAssistInterface const &graphicalModel
			, ErrorReporterInterface &errorReporter
			, GeneratorCustomizer const &customizer
			, Id const &diagramId
			, QObject *parent = 0);
	virtual ~ControlFlowGeneratorBase();

	bool preGenerationCheck();

	virtual ControlFlow *generate();

protected:
	void error(QString const &message, Id const &id = Id());
	enums::semantics::Semantics semanticsOf(Id const &id) const;
	Id initialNode() const;
	QPair<LinkInfo, LinkInfo> ifBranchesFor(Id const &id) const;
	QPair<LinkInfo, LinkInfo> loopBranchesFor(Id const &id) const;

	GeneratorCustomizer const &customizer() const;

private:
	LogicalModelAssistInterface const &mModel;
	ErrorReporterInterface &mErrorReporter;
	GeneratorCustomizer const &mCustomizer;
	Id const mDiagram;
	PrimaryControlFlowValidator mValidator;
};

}
}
}
