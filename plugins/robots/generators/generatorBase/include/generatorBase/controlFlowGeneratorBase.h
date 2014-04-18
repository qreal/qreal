#pragma once

#include "robotsGeneratorDeclSpec.h"
#include "primaryControlFlowValidator.h"
#include "robotsDiagramVisitor.h"
#include "semanticTree/semanticTree.h"

namespace qReal {
namespace robots {
namespace generators {

/// A base class for generators that build sementic tree from model in repo
class ROBOTS_GENERATOR_EXPORT ControlFlowGeneratorBase : public QObject, public RobotsDiagramVisitor
{
	Q_OBJECT

public:
	/// @param isThisDiagramMain 'true' if this generator generates code for main diagram
	/// (main diagram is the one which was active when user requested generation)
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

	/// Copies this generator and returns new instance which is owned by the same
	/// parent. Implementation must pay attention to isThisDiagramMain parameter
	/// (it should be always false in copied objects)
	virtual ControlFlowGeneratorBase *cloneFor(Id const &diagramId) = 0;

	/// Generates control flow object representation (SemanticTree) and returns
	/// a pointer to it if generation process was successfull or NULL otherwise.
	/// Takes ownership on result.
	virtual semantics::SemanticTree *generate();

	/// Returns true if some generation errors occured and the generation process can`t be proceeded with other
	/// control flow generators (fatal errors occured).
	bool errorsOccured() const;

protected:
	void error(QString const &message, Id const &id = Id(), bool critical = true);

	enums::semantics::Semantics semanticsOf(Id const &id) const;
	Id initialNode() const;
	QPair<LinkInfo, LinkInfo> ifBranchesFor(Id const &id) const;
	QPair<LinkInfo, LinkInfo> loopBranchesFor(Id const &id) const;

	GeneratorCustomizer &customizer() const;

	semantics::SemanticTree *mSemanticTree;  // Takes ownership
	qrRepo::RepoApi const &mRepo;
	ErrorReporterInterface &mErrorReporter;
	GeneratorCustomizer &mCustomizer;
	bool mErrorsOccured;
	bool const mIsMainGenerator;

private:
	Id const mDiagram;
	PrimaryControlFlowValidator mValidator;
};

}
}
}
