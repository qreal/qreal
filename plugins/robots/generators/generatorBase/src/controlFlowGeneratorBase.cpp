#include "generatorBase/controlFlowGeneratorBase.h"

#include "generatorBase/semanticTree/semanticTree.h"
#include "generatorBase/parts/threads.h"
#include "generatorBase/parts/subprograms.h"

#include "src/rules/forkRules/forkRule.h"

using namespace generatorBase;
using namespace qReal;

ControlFlowGeneratorBase::ControlFlowGeneratorBase(
		const qrRepo::RepoApi &repo
		, ErrorReporterInterface &errorReporter
		, GeneratorCustomizer &customizer
		, const Id &diagramId
		, QObject *parent
		, bool isThisDiagramMain)
	: QObject(parent)
	, RobotsDiagramVisitor(repo, customizer)
	, mRepo(repo)
	, mErrorReporter(errorReporter)
	, mCustomizer(customizer)
	, mIsMainGenerator(isThisDiagramMain)
	, mDiagram(diagramId)
	, mValidator(new PrimaryControlFlowValidator(repo, errorReporter, customizer, diagramId, this))
{
}

ControlFlowGeneratorBase::~ControlFlowGeneratorBase()
{
}

bool ControlFlowGeneratorBase::preGenerationCheck()
{
	return mValidator->validate();
}

semantics::SemanticTree *ControlFlowGeneratorBase::generate(const qReal::Id &initialNode)
{
	// If initial node is non-null then pregeneration check was already performed;
	if (initialNode.isNull() && !preGenerationCheck()) {
		mSemanticTree = nullptr;
		return nullptr;
	}

	const qReal::Id realInitialNode = initialNode.isNull() ? this->initialNode() : initialNode;
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
		const Id thread = mCustomizer.factory()->threads().nextUnprocessedThread();
		ControlFlowGeneratorBase * const threadGenerator = this->cloneFor(thread, false);
		if (!threadGenerator->generate(thread)) {
			return false;
		}
	}

	return true;
}

void ControlFlowGeneratorBase::error(const QString &message, const Id &id, bool critical)
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

void ControlFlowGeneratorBase::visitRegular(const Id &id, QList<LinkInfo> const &links)
{
	Q_UNUSED(links)
	if (mCustomizer.isSubprogramCall(id)) {
		mCustomizer.factory()->subprograms()->usageFound(id);
	}
}

enums::semantics::Semantics ControlFlowGeneratorBase::semanticsOf(const qReal::Id &id) const
{
	return mCustomizer.semanticsOf(id.type());
}

qReal::Id ControlFlowGeneratorBase::initialNode() const
{
	return mValidator->initialNode();
}

QPair<LinkInfo, LinkInfo> ControlFlowGeneratorBase::ifBranchesFor(const qReal::Id &id) const
{
	return mValidator->ifBranchesFor(id);
}

QPair<LinkInfo, LinkInfo> ControlFlowGeneratorBase::loopBranchesFor(const qReal::Id &id) const
{
	return mValidator->loopBranchesFor(id);
}

GeneratorCustomizer &ControlFlowGeneratorBase::customizer() const
{
	return mCustomizer;
}

void ControlFlowGeneratorBase::visitFinal(const Id &id, QList<LinkInfo> const &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
}

void ControlFlowGeneratorBase::visitFork(const Id &id, QList<LinkInfo> &links)
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
