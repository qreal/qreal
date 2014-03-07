#include "gotoControlFlowGenerator.h"

using namespace qReal::robots::generators;
using namespace semantics;

GotoControlFlowGenerator::GotoControlFlowGenerator(
		qrRepo::RepoApi const &repo
		, ErrorReporterInterface &errorReporter
		, GeneratorCustomizer &customizer
		, Id const &diagramId
		, QObject *parent
		, bool isThisDiagramMain)
	: ControlFlowGeneratorBase(repo, errorReporter, customizer, diagramId, parent, isThisDiagramMain)
{
}

ControlFlowGeneratorBase *GotoControlFlowGenerator::cloneFor(qReal::Id const &diagramId)
{
	return new GotoControlFlowGenerator(mRepo, mErrorReporter, mCustomizer
			, diagramId, parent(), false);
}

void GotoControlFlowGenerator::beforeSearch()
{
}

void GotoControlFlowGenerator::visitRegular(Id const &id, QList<LinkInfo> const &links)
{
	SimpleNode * const thisNode = static_cast<SimpleNode *>(mSemanticTree->findNodeFor(id));
	SemanticNode *nextNode = nullptr;
	if (!links[0].targetVisited) {
		nextNode = mSemanticTree->produceNodeFor(links[0].target);
		nextNode->addLabel();
	} else {
		nextNode = produceGotoNode(links[0].target);
	}

	thisNode->insertSiblingAfterThis(nextNode);
}

void GotoControlFlowGenerator::visitFinal(Id const &id, QList<LinkInfo> const &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
}

void GotoControlFlowGenerator::visitConditional(Id const &id, QList<LinkInfo> const &links)
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

void GotoControlFlowGenerator::visitLoop(Id const &id, QList<LinkInfo> const &links)
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

void GotoControlFlowGenerator::visitSwitch(Id const &id, QList<LinkInfo> const &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
}

void GotoControlFlowGenerator::visitFork(Id const &id, QList<LinkInfo> const &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
}

void GotoControlFlowGenerator::afterSearch()
{
}

SemanticNode *GotoControlFlowGenerator::produceGotoNode(qReal::Id const &id)
{
	// Using other id to eliminate collisions with original node
	Id const gotoId("label_" + id.editor(), id.diagram(), id.element(), id.id());
	SimpleNode * const result = mSemanticTree->produceSimple(gotoId);
	result->bindToSyntheticConstruction(SimpleNode::gotoNode);
	return result;
}

void GotoControlFlowGenerator::produceNextNodeIfNeeded(LinkInfo const &info, NonZoneNode * const parent)
{
	if (!info.targetVisited) {
		SemanticNode * const nextNode = mSemanticTree->produceNodeFor(info.target);
		nextNode->addLabel();
		parent->appendSibling(nextNode);
	}
}
