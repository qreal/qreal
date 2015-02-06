#include "gotoControlFlowGenerator.h"

using namespace generatorBase;
using namespace qReal;
using namespace semantics;

GotoControlFlowGenerator::GotoControlFlowGenerator(
		const qrRepo::RepoApi &repo
		, ErrorReporterInterface &errorReporter
		, GeneratorCustomizer &customizer
		, const Id &diagramId
		, QObject *parent
		, bool isThisDiagramMain)
	: ControlFlowGeneratorBase(repo, errorReporter, customizer, diagramId, parent, isThisDiagramMain)
{
}

ControlFlowGeneratorBase *GotoControlFlowGenerator::cloneFor(const qReal::Id &diagramId, bool cloneForNewDiagram)
{
	GotoControlFlowGenerator * const copy = new GotoControlFlowGenerator(mRepo
			, mErrorReporter, mCustomizer, diagramId, parent(), false);
	if (!cloneForNewDiagram) {
		delete copy->mValidator;
		copy->mValidator = mValidator;
	}

	return copy;
}

void GotoControlFlowGenerator::beforeSearch()
{
}

void GotoControlFlowGenerator::visitRegular(const Id &id, QList<LinkInfo> const &links)
{
	ControlFlowGeneratorBase::visitRegular(id, links);
	SimpleNode * const thisNode = static_cast<SimpleNode *>(mSemanticTree->findNodeFor(id));
	SemanticNode *nextNode = nullptr;
	if (mSemanticTree->findNodeFor(links[0].target)) {
		nextNode = produceGotoNode(links[0].target);
	} else {
		nextNode = mSemanticTree->produceNodeFor(links[0].target);
		nextNode->addLabel();
	}

	thisNode->insertSiblingAfterThis(nextNode);
}

void GotoControlFlowGenerator::visitConditional(const Id &id, QList<LinkInfo> const &links)
{
	Q_UNUSED(links)

	QPair<LinkInfo, LinkInfo> const branches(ifBranchesFor(id));
	LinkInfo const thenLink = branches.first;
	LinkInfo const elseLink = branches.second;

	IfNode * const thisNode = static_cast<IfNode *>(mSemanticTree->findNodeFor(id));
	thisNode->thenZone()->appendChild(produceGotoNode(thenLink.target));
	thisNode->elseZone()->appendChild(produceGotoNode(elseLink.target));

	produceNextNodeIfNeeded(thenLink, thisNode);
	produceNextNodeIfNeeded(elseLink, thisNode);
}

void GotoControlFlowGenerator::visitLoop(const Id &id, QList<LinkInfo> const &links)
{
	Q_UNUSED(links)

	QPair<LinkInfo, LinkInfo> const branches(loopBranchesFor(id));
	LinkInfo const iterationLink = branches.first;
	LinkInfo const nextLink = branches.second;

	LoopNode * const thisNode = static_cast<LoopNode *>(mSemanticTree->findNodeFor(id));
	thisNode->bodyZone()->appendChild(produceGotoNode(iterationLink.target));
	thisNode->insertSiblingAfterThis(produceGotoNode(nextLink.target));

	produceNextNodeIfNeeded(iterationLink, thisNode);
	produceNextNodeIfNeeded(nextLink, thisNode);
}

void GotoControlFlowGenerator::visitSwitch(const Id &id, QList<LinkInfo> const &links)
{
	SwitchNode * const thisNode = static_cast<SwitchNode *>(mSemanticTree->findNodeFor(id));
	for (LinkInfo const &branch : links) {
		const QString value = mRepo.property(branch.linkId, "Guard").toString();
		thisNode->addBranch(value, produceGotoNode(branch.target));
		produceNextNodeIfNeeded(branch, thisNode);
	}
}

void GotoControlFlowGenerator::afterSearch()
{
}

SemanticNode *GotoControlFlowGenerator::produceGotoNode(const qReal::Id &id)
{
	// Using other id to eliminate collisions with original node
	const Id gotoId("label_" + id.editor(), id.diagram(), id.element(), id.id());
	SimpleNode * const result = mSemanticTree->produceSimple(gotoId);
	result->bindToSyntheticConstruction(SimpleNode::gotoNode);
	return result;
}

void GotoControlFlowGenerator::produceNextNodeIfNeeded(LinkInfo const &info, NonZoneNode * const parent)
{
	if (!mSemanticTree->findNodeFor(info.target)) {
		SemanticNode * const nextNode = mSemanticTree->produceNodeFor(info.target);
		nextNode->addLabel();
		parent->appendSibling(nextNode);
	}
}
