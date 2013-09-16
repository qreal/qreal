#pragma once

#include "primaryControlFlowValidator.h"
#include "robotsDiagramVisitor.h"
#include "semanticTree/semanticTree.h"

namespace qReal {
namespace robots {
namespace generators {

/// A base class for generators that build sementic tree from model in repo
class ControlFlowGeneratorBase : public QObject, public RobotsDiagramVisitor
{
public:
	/// @param isThisDiagramMain 'true' if this generator generates code for main diagram
	/// (main diagram is the one whitch was active when user requested generation)
	ControlFlowGeneratorBase(
			qrRepo::RepoApi const &repo
			, ErrorReporterInterface &errorReporter
			, GeneratorCustomizer &customizer
			, Id const &diagramId
			, QObject *parent = 0
			, bool isThisDiagramMain = true);
	virtual ~ControlFlowGeneratorBase();

	/// Validates diagram checking if given model in repo satisfies the simplest
	/// conditions (like all links are connected and correctly marked and so on)
	bool preGenerationCheck();

	/// Copies this generator and returns new instance whitch is owned by the same
	/// parent. Implementation must pay attention to isThisDiagramMain parameter
	/// (it should be always false in copied objects)
	virtual ControlFlowGeneratorBase *cloneFor(Id const &diagramId) = 0;

	/// Generates control flow object representation (SemanticTree) and returns
	/// a pointer to it if generation process was successfull or NULL otherwise.
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
