#pragma once

#include "robotsGeneratorDeclSpec.h"
#include "primaryControlFlowValidator.h"
#include "robotsDiagramVisitor.h"
#include "semanticTree/semanticTree.h"

namespace generatorBase {

/// A base class for generators that builds sementic tree from model in repo
class ROBOTS_GENERATOR_EXPORT ControlFlowGeneratorBase : public QObject, public RobotsDiagramVisitor
{
	Q_OBJECT

public:
	/// @param isThisDiagramMain 'true' if this generator generates code for main diagram
	/// (main diagram is the one which was active when user requested generation)
	ControlFlowGeneratorBase(
			qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, GeneratorCustomizer &customizer
			, qReal::Id const &diagramId
			, QObject *parent = 0
			, bool isThisDiagramMain = true);
	virtual ~ControlFlowGeneratorBase();

	/// Validates diagram checking if given model in repo satisfies the simplest
	/// conditions (like all links are connected and correctly marked and so on)
	bool preGenerationCheck();

	/// Copies this generator and returns new instance which is owned by the same
	/// parent. Implementation must pay attention to isThisDiagramMain parameter
	/// (it should be always false in copied objects).
	/// Ownership on the cloned generators is taken by this generator`s parent.
	virtual ControlFlowGeneratorBase *cloneFor(qReal::Id const &diagramId) = 0;

	/// Generates control flow object representation (SemanticTree) and returns
	/// a pointer to it if generation process was successfull or NULL otherwise.
	/// Takes ownership on result.
	/// @param initialNode The starting block of the traversal. If empty then initial node
	/// of the diagram given in constructor will be used.
	semantics::SemanticTree *generate(qReal::Id const &initialNode = qReal::Id());

	/// Returns true if some generation errors occured and the generation process can`t be proceeded with other
	/// control flow generators (fatal errors occured).
	bool errorsOccured() const;

	void visitFinal(qReal::Id const &id, QList<LinkInfo> const &links) override;
	void visitFork(qReal::Id const &id, QList<LinkInfo> &links) override;

protected:
	/// Can be overloaded by ancestors for custom behaviour.
	virtual void performGeneration();

	bool generateForks();

	void error(QString const &message, qReal::Id const &id = qReal::Id(), bool critical = true);

	enums::semantics::Semantics semanticsOf(qReal::Id const &id) const;
	qReal::Id initialNode() const;
	QPair<LinkInfo, LinkInfo> ifBranchesFor(qReal::Id const &id) const;
	QPair<LinkInfo, LinkInfo> loopBranchesFor(qReal::Id const &id) const;

	GeneratorCustomizer &customizer() const;

	semantics::SemanticTree *mSemanticTree;  // Takes ownership
	qrRepo::RepoApi const &mRepo;
	qReal::ErrorReporterInterface &mErrorReporter;
	GeneratorCustomizer &mCustomizer;
	bool mErrorsOccured;
	bool const mIsMainGenerator;

	qReal::Id const mDiagram;
	PrimaryControlFlowValidator *mValidator;  // Takes owneship via Qt parentship system
};

}
