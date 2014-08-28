#include "generatorBase/controlFlowGeneratorBase.h"

#include "generatorBase/semanticTree/semanticTree.h"
#include "generatorBase/parts/threads.h"

#include "src/rules/forkRules/forkRule.h"

using namespace generatorBase;
using namespace qReal;

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

semantics::SemanticTree *ControlFlowGeneratorBase::generate(qReal::Id const &initialNode)
{
	// If initial node is non-null then pregeneration check was already performed;
	if (initialNode.isNull() && !preGenerationCheck()) {
		mSemanticTree = nullptr;
		return nullptr;
	}

	qReal::Id const realInitialNode = initialNode.isNull() ? this->initialNode() : initialNode;
	mSemanticTree = new semantics::SemanticTree(customizer(), realInitialNode, mIsMainGenerator, this);
	mCustomizer.factory()->threads().threadProcessed(realInitialNode, *mSemanticTree);
	mErrorsOccured = false;

	performGeneration();

	mErrorsOccured &= generateForks();
	if (mErrorsOccured) {
		mSemanticTree = nullptr;
	}

	return mSemanticTree;
}

void ControlFlowGeneratorBase::performGeneration()
{
	// This will start dfs on model graph with processing every block
	// in subclasses which must construct control flow in handlers
	startSearch(mSemanticTree->initialBlock());
}

bool ControlFlowGeneratorBase::generateForks()
{
	while (mCustomizer.factory()->threads().hasUnprocessedThreads()) {
		Id const thread = mCustomizer.factory()->threads().nextUnprocessedThread();
		ControlFlowGeneratorBase * const threadGenerator = this->cloneFor(thread);
		if (!threadGenerator->generate(thread)) {
			return false;
		}
	}

	return true;
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

void ControlFlowGeneratorBase::visitFinal(Id const &id, QList<LinkInfo> const &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
}

void ControlFlowGeneratorBase::visitSwitch(Id const &id, QList<LinkInfo> const &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
	error(tr("Switches are not supported in generator yet"), id, true);
}

void ControlFlowGeneratorBase::visitFork(Id const &id, QList<LinkInfo> &links)
{
	// n-ary fork creates (n-1) new threads and one thread is the old one.
	LinkInfo const currentThread = links.first();
	// In case of current thread fork block behaviours like nop-block.
	visitRegular(id, { currentThread });
	QList<LinkInfo> const newThreads = links.mid(1);
	semantics::ForkRule rule(mSemanticTree, id, newThreads, mCustomizer.factory()->threads());
	rule.apply();

	// Restricting visiting other threads, they will be generated to new semantic trees.
	links = {currentThread};
}
