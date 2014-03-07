#include "controlFlowGeneratorBase.h"

using namespace qReal::robots::generators;

ControlFlowGeneratorBase::ControlFlowGeneratorBase(
		qrRepo::RepoApi const &repo
		, ErrorReporterInterface &errorReporter
		, GeneratorCustomizer &customizer
		, Id const &diagramId
		, QObject *parent
		, bool isThisDiagramMain)
	: QObject(parent)
	, RobotsDiagramVisitor(repo, customizer)
	, mRepo(repo)
	, mErrorReporter(errorReporter)
	, mCustomizer(customizer)
	, mIsMainGenerator(isThisDiagramMain)
	, mDiagram(diagramId)
	, mValidator(repo, errorReporter, customizer, diagramId)
{
}

ControlFlowGeneratorBase::~ControlFlowGeneratorBase()
{
}

bool ControlFlowGeneratorBase::preGenerationCheck()
{
	return mValidator.validate();
}

semantics::SemanticTree *ControlFlowGeneratorBase::generate()
{
	if (!preGenerationCheck()) {
		mSemanticTree = NULL;
		return NULL;
	}

	mErrorsOccured = false;
	mSemanticTree = new semantics::SemanticTree(customizer(), initialNode(), mIsMainGenerator, this);

	// This will start dfs on model graph with processing every block
	// in subclasses which must construct control flow in handlers
	startSearch(initialNode());

	if (mErrorsOccured) {
		mSemanticTree = NULL;
	}

	return mSemanticTree;
}

void ControlFlowGeneratorBase::error(QString const &message, Id const &id, bool critical)
{
	mErrorsOccured = true;
	if (critical) {
		mErrorReporter.addCritical(message, id);
		terminateSearch();
	} else {
		mErrorReporter.addError(message, id);
	}
}

bool ControlFlowGeneratorBase::errorsOccured() const
{
	return mErrorsOccured;
}

enums::semantics::Semantics ControlFlowGeneratorBase::semanticsOf(qReal::Id const &id) const
{
	return mCustomizer.semanticsOf(id.type());
}

qReal::Id ControlFlowGeneratorBase::initialNode() const
{
	return mValidator.initialNode();
}

QPair<LinkInfo, LinkInfo> ControlFlowGeneratorBase::ifBranchesFor(qReal::Id const &id) const
{
	return mValidator.ifBranchesFor(id);
}

QPair<LinkInfo, LinkInfo> ControlFlowGeneratorBase::loopBranchesFor(qReal::Id const &id) const
{
	return mValidator.loopBranchesFor(id);
}

GeneratorCustomizer &ControlFlowGeneratorBase::customizer() const
{
	return mCustomizer;
}
