#pragma once

#include "primaryControlFlowValidator.h"
#include "robotsDiagramVisitor.h"
#include "semanticTree/semanticTree.h"

namespace qReal {
namespace robots {
namespace generators {

class ControlFlowGeneratorBase : public QObject, public RobotsDiagramVisitor
{
public:
	ControlFlowGeneratorBase(
			qrRepo::RepoApi const &repo
			, ErrorReporterInterface &errorReporter
			, GeneratorCustomizer &customizer
			, Id const &diagramId
			, QObject *parent = 0
			, bool isThisDiagramMain = true);
	virtual ~ControlFlowGeneratorBase();

	bool preGenerationCheck();

	virtual ControlFlowGeneratorBase *cloneFor(Id const &diagramId) = 0;

	virtual semantics::SemanticTree *generate();

protected:
	void error(QString const &message, Id const &id = Id(), bool critical = true);
	bool errorsOccured() const;

	enums::semantics::Semantics semanticsOf(Id const &id) const;
	Id initialNode() const;
	QPair<LinkInfo, LinkInfo> ifBranchesFor(Id const &id) const;
	QPair<LinkInfo, LinkInfo> loopBranchesFor(Id const &id) const;

	GeneratorCustomizer &customizer() const;

	semantics::SemanticTree *mSemanticTree;  // Takes ownership
	qrRepo::RepoApi const &mRepo;
	ErrorReporterInterface &mErrorReporter;
	GeneratorCustomizer &mCustomizer;

private:
	Id const mDiagram;
	bool const mIsMainGenerator;
	PrimaryControlFlowValidator mValidator;
	bool mErrorsOccured;
};

}
}
}
