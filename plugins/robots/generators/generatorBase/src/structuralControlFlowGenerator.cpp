/* Copyright 2018 Konstantin Batoev
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */
#include "structuralControlFlowGenerator.h"

#include <QtCore/QQueue>

#include <QStack>

#include "structurizatorNodes/intermediateStructurizatorNode.h"
#include "structurizatorNodes/blockStructurizatorNode.h"
#include "structurizatorNodes/breakStructurizatorNode.h"
#include "structurizatorNodes/ifStructurizatorNode.h"
#include "structurizatorNodes/structurizatorNodeWithBreaks.h"
#include "structurizatorNodes/selfLoopStructurizatorNode.h"
#include "structurizatorNodes/simpleStructurizatorNode.h"
#include "structurizatorNodes/switchStructurizatorNode.h"
#include "structurizatorNodes/whileStructurizatorNode.h"

#include "generatorBase/parts/subprograms.h"
#include "generatorBase/parts/threads.h"

using namespace qReal;
using namespace generatorBase;
using namespace semantics;


StructuralControlFlowGenerator::StructuralControlFlowGenerator(const qrRepo::RepoApi &repo
		, ErrorReporterInterface &errorReporter
		, GeneratorCustomizer &customizer
		, PrimaryControlFlowValidator &validator
		, const Id &diagramId
		, QObject *parent
		, bool isThisDiagramMain
		, Id simpleId)
	: ControlFlowGeneratorBase(repo, errorReporter, customizer, validator, diagramId, parent, isThisDiagramMain)
	, mCantBeGeneratedIntoStructuredCode(false)
	, mStructurizator(new Structurizator(this))
	, mVerticesNumber(0)
	, mStartVertex(0)
	, mIsGraphBeingConstructed(true)
	, mSimpleId(simpleId)
{
}

ControlFlowGeneratorBase *StructuralControlFlowGenerator::cloneFor(const Id &diagramId, bool cloneForNewDiagram)
{
	StructuralControlFlowGenerator * const copy = new StructuralControlFlowGenerator(mRepo
			, mErrorReporter, mCustomizer, (cloneForNewDiagram ? *mValidator.clone() : mValidator)
			, diagramId, parent(), false, mSimpleId);

	return copy;
}

void StructuralControlFlowGenerator::beforeSearch()
{
}

void StructuralControlFlowGenerator::visit(const Id &id, QList<LinkInfo> &links)
{
	if (mSimpleId == qReal::Id()) {
		mSimpleId = id.sameTypeId();
	}

	mWasDoneThisIteration = false;

	ControlFlowGeneratorBase::visit(id, links);

	if (!mWasDoneThisIteration) {
		appendEdgesAndVertices(id, links);
		addVerticesInLoopBody(id, links);
	}
}

void StructuralControlFlowGenerator::afterVisit(const Id &id, QList<LinkInfo> &links)
{
	if (!mIsGraphBeingConstructed) {
		return;
	}

	if (semanticsOf(id) == enums::semantics::loopBlock) {
		mLoopNumbers.pop();

		QPair<LinkInfo, LinkInfo> loopBranches = loopBranchesFor(id);
		mVerticesInsideLoopBody.remove(mVertexNumber[loopBranches.first.target]);
	}

	removeVerticesFromLoopBody(id, links);
}

void StructuralControlFlowGenerator::visitConditional(const Id &id, const QList<LinkInfo> &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
}

void StructuralControlFlowGenerator::visitLoop(const Id &id, const QList<LinkInfo> &links)
{
	if (!mIsGraphBeingConstructed) {
		return;
	}

	appendEdgesAndVertices(id, links);
	addVerticesInLoopBody(id, links);

	mWasDoneThisIteration = true;

	mLoopNumbers.push(mVertexNumber[id]);

	QPair<LinkInfo, LinkInfo> loopBranches = loopBranchesFor(id);
	mVerticesInsideLoopBody.insert(mVertexNumber[loopBranches.first.target]);
}

void StructuralControlFlowGenerator::visitSwitch(const Id &id, const QList<LinkInfo> &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
}

void StructuralControlFlowGenerator::visitUnknown(const Id &id, const QList<LinkInfo> &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
}

void StructuralControlFlowGenerator::afterSearch()
{
}

bool StructuralControlFlowGenerator::cantBeGeneratedIntoStructuredCode() const
{
	return mCantBeGeneratedIntoStructuredCode;
}

void StructuralControlFlowGenerator::performGeneration()
{
	ControlFlowGeneratorBase::performGeneration();
	IntermediateStructurizatorNode *tree = mStructurizator->performStructurization(mIds
			, mStartVertex, mFollowers, mVertexNumber, mVerticesNumber);

	if (tree) {
		obtainSemanticTree(tree);
		mIsGraphBeingConstructed = false;
		ControlFlowGeneratorBase::performGeneration();
	} else {
		mCantBeGeneratedIntoStructuredCode = true;
	}

	if (mCantBeGeneratedIntoStructuredCode) {
		mSemanticTree = nullptr;
	}
}

void StructuralControlFlowGenerator::registerOtherThreads(const Id &id, const QList<LinkInfo> &threads
		, const QHash<Id, QString> &threadIds, parts::Threads &threadsStorage)
{
	if (!mIsGraphBeingConstructed) {
		ControlFlowGeneratorBase::registerOtherThreads(id, threads, threadIds, threadsStorage);
	}
}

void StructuralControlFlowGenerator::registerTerminatingThreads(const Id &id, parts::Threads &threadsStorage
		, bool fromMain)
{
	if (!mIsGraphBeingConstructed) {
		ControlFlowGeneratorBase::registerTerminatingThreads(id, threadsStorage, fromMain);
	}
}

void StructuralControlFlowGenerator::obtainSemanticTree(IntermediateStructurizatorNode *root)
{
	root->analyzeBreak();
	mSemanticTree->setRoot(transformNode(root));
}

void StructuralControlFlowGenerator::checkAndAppendBlock(ZoneNode *zone, IntermediateStructurizatorNode *node)
{
	if (node->type() == IntermediateStructurizatorNode::simple) {
		SimpleStructurizatorNode *simpleNode = static_cast<SimpleStructurizatorNode *>(node);

		switch (semanticsOf(simpleNode->id())) {
		case enums::semantics::conditionalBlock:
		case enums::semantics::switchBlock:
			break;
		default:
			zone->appendChild(transformSimple(simpleNode));
		}
	} else {
		zone->appendChild(transformNode(node));
	}
}

// maybe use strategy to recursively handle this situation?
SemanticNode *StructuralControlFlowGenerator::transformNode(IntermediateStructurizatorNode *node)
{
	switch (node->type()) {
	case IntermediateStructurizatorNode::Type::simple: {
		SimpleStructurizatorNode *simpleNode = static_cast<SimpleStructurizatorNode *>(node);
		return transformSimple(simpleNode);
	}

	case IntermediateStructurizatorNode::Type::block: {
		BlockStructurizatorNode *blockNode = static_cast<BlockStructurizatorNode *>(node);
		return transformBlock(blockNode);
	}

	case IntermediateStructurizatorNode::Type::ifThenElseCondition: {
		IfStructurizatorNode *ifNode = static_cast<IfStructurizatorNode *>(node);
		return transformIfThenElse(ifNode);
	}

	case IntermediateStructurizatorNode::Type::switchCondition: {
		SwitchStructurizatorNode *switchNode = static_cast<SwitchStructurizatorNode *>(node);
		return transformSwitch(switchNode);
	}

	case IntermediateStructurizatorNode::Type::infiniteloop: {
		SelfLoopStructurizatorNode *selfLoopNode = static_cast<SelfLoopStructurizatorNode *>(node);
		return transformSelfLoop(selfLoopNode);
	}

	case IntermediateStructurizatorNode::Type::whileloop: {
		WhileStructurizatorNode *whileNode = static_cast<WhileStructurizatorNode *>(node);
		return transformWhileLoop(whileNode);
	}

	case IntermediateStructurizatorNode::Type::breakNode: {
		return transformBreakNode();
	}

	case IntermediateStructurizatorNode::Type::nodeWithBreaks: {
		return createConditionWithBreaks(static_cast<StructurizatorNodeWithBreaks *>(node));
	}

	default:
		mCantBeGeneratedIntoStructuredCode = true;
		return mSemanticTree->produceSimple();
	}
}

SemanticNode *StructuralControlFlowGenerator::transformSimple(SimpleStructurizatorNode *simpleNode)
{
	return mSemanticTree->produceNodeFor(simpleNode->id());
}

SemanticNode *StructuralControlFlowGenerator::transformBlock(BlockStructurizatorNode *blockNode)
{
	ZoneNode *zone = new ZoneNode(mSemanticTree);
	checkAndAppendBlock(zone, blockNode->firstNode());
	checkAndAppendBlock(zone, blockNode->secondNode());

	return zone;
}

SemanticNode *StructuralControlFlowGenerator::transformIfThenElse(IfStructurizatorNode *ifNode)
{
	if (ifNode->condition()->type() == IntermediateStructurizatorNode::nodeWithBreaks) {
		StructurizatorNodeWithBreaks *nodeWithBreaks =
				static_cast<StructurizatorNodeWithBreaks *>(ifNode->condition());
		nodeWithBreaks->setRestBranches({ifNode->thenBranch(), ifNode->elseBranch()});
		return createConditionWithBreaks(nodeWithBreaks);
	}

	const qReal::Id conditionId = ifNode->condition()->firstId();

	switch (semanticsOf(conditionId)) {

	case enums::semantics::conditionalBlock: {
		return createSemanticIfNode(conditionId, ifNode->thenBranch(), ifNode->elseBranch());
	}

	case enums::semantics::switchBlock: {
		QList<IntermediateStructurizatorNode *> branches = { ifNode->thenBranch() };

		if (ifNode->elseBranch()) {
			branches.append(ifNode->elseBranch());
		}

		return createSemanticSwitchNode(conditionId, branches, ifNode->hasBreakInside());
	}

	case enums::semantics::loopBlock: {
		if ((ifNode->exit() && ifNode->elseBranch() && ifNode->exit()->firstId() == ifNode->firstId())
				|| (!ifNode->exit() && ifNode->elseBranch())) {
			ZoneNode *zone = new ZoneNode(mSemanticTree);
			const qReal::Id loopCondition = ifNode->condition()->firstId();
			LoopNode *innerLoop = mSemanticTree->produceLoop(loopCondition);

			QPair<LinkInfo, LinkInfo> loopBranches = loopBranchesFor(loopCondition);
			IntermediateStructurizatorNode *restBranch = ifNode->thenBranch();

			if (ifNode->thenBranch()->firstId() == loopBranches.first.target) {
				innerLoop->bodyZone()->appendChild(transformNode(ifNode->thenBranch()));
				restBranch = ifNode->elseBranch();
			} else {
				innerLoop->bodyZone()->appendChild(transformNode(ifNode->elseBranch()));
			}

			zone->appendChild(innerLoop);
			zone->appendChild(transformNode(restBranch));
			return zone;
		}
	}

	default:
		break;
	}

	mCantBeGeneratedIntoStructuredCode = true;

	return mSemanticTree->produceSimple();
}

SemanticNode *StructuralControlFlowGenerator::transformSelfLoop(SelfLoopStructurizatorNode *selfLoopNode)
{
	LoopNode *semanticLoop = mSemanticTree->produceLoop();
	semanticLoop->bodyZone()->appendChild(transformNode(selfLoopNode->bodyNode()));
	return semanticLoop;
}

SemanticNode *StructuralControlFlowGenerator::transformWhileLoop(WhileStructurizatorNode *whileNode)
{
	IntermediateStructurizatorNode *headNode = whileNode->headNode();
	IntermediateStructurizatorNode *bodyNode = whileNode->bodyNode();
	IntermediateStructurizatorNode *exitNode = whileNode->exitNode();

	LoopNode *semanticLoop = nullptr;
	const qReal::Id conditionId = headNode->firstId();

	if (headNode->type() == IntermediateStructurizatorNode::Type::simple) {
		switch (semanticsOf(conditionId)) {
		case enums::semantics::conditionalBlock: {
			if (ifBranchesFor(conditionId).first.target == bodyNode->firstId()) {
				semanticLoop = mSemanticTree->produceLoop(conditionId);
			} else {
				semanticLoop = mSemanticTree->produceLoop();
				IfNode *conditionNode = mSemanticTree->produceConditional(conditionId);
				conditionNode->thenZone()->appendChild(SimpleNode::createBreakNode(mSemanticTree));
				semanticLoop->bodyZone()->appendChild(conditionNode);
			}

			semanticLoop->bodyZone()->appendChild(transformNode(bodyNode));
			return semanticLoop;
		}

		case enums::semantics::loopBlock: {
			semanticLoop = mSemanticTree->produceLoop(conditionId);
			semanticLoop->bodyZone()->appendChild(transformNode(bodyNode));
			return semanticLoop;
		}

		case enums::semantics::switchBlock: {
			QList<IntermediateStructurizatorNode *> exitBranches;
			exitBranches.append(new BreakStructurizatorNode(exitNode->firstId(), mStructurizator));

			StructurizatorNodeWithBreaks *nodeWithBreaks = new StructurizatorNodeWithBreaks(headNode
					, exitBranches, mStructurizator);
			nodeWithBreaks->setRestBranches( { bodyNode } );

			semanticLoop = mSemanticTree->produceLoop();
			semanticLoop->bodyZone()->appendChild(createConditionWithBreaks(nodeWithBreaks));
			return semanticLoop;
		}

		default:
			break;
		}
	} else if (headNode->type() == IntermediateStructurizatorNode::Type::nodeWithBreaks
					&& semanticsOf(conditionId) == enums::semantics::loopBlock) {
		StructurizatorNodeWithBreaks *nodeWitBreaks = static_cast<StructurizatorNodeWithBreaks *>(headNode);
		if (nodeWitBreaks->exitBranches().size() != 1 ||
					nodeWitBreaks->exitBranches().first()->type() == IntermediateStructurizatorNode::block) {
			mCantBeGeneratedIntoStructuredCode = true;
			return mSemanticTree->produceSimple();
		} else {
			semanticLoop = mSemanticTree->produceLoop(conditionId);
			semanticLoop->bodyZone()->appendChild(transformNode(bodyNode));
			return semanticLoop;
		}
	}

	semanticLoop = mSemanticTree->produceLoop();
	semanticLoop->bodyZone()->appendChild(transformNode(headNode));
	semanticLoop->bodyZone()->appendChild(transformNode(bodyNode));
	return semanticLoop;
}

SemanticNode *StructuralControlFlowGenerator::transformSwitch(SwitchStructurizatorNode *switchNode)
{
	const qReal::Id &conditionId = switchNode->condition()->firstId();
	QList<IntermediateStructurizatorNode *> branches = switchNode->branches();

	if (switchNode->condition()->type() == IntermediateStructurizatorNode::nodeWithBreaks) {
		StructurizatorNodeWithBreaks *nodeWithBreaks =
				static_cast<StructurizatorNodeWithBreaks *>(switchNode->condition());
		nodeWithBreaks->setRestBranches(branches);
		return createConditionWithBreaks(nodeWithBreaks);
	}

	if (semanticsOf(conditionId) == enums::semantics::switchBlock) {
		return createSemanticSwitchNode(conditionId, branches, switchNode->hasBreakInside());
	}

	mCantBeGeneratedIntoStructuredCode = true;

	return mSemanticTree->produceSimple();
}

SemanticNode *StructuralControlFlowGenerator::transformBreakNode()
{
	return semantics::SimpleNode::createBreakNode(mSemanticTree);
}

SemanticNode *StructuralControlFlowGenerator::createConditionWithBreaks(StructurizatorNodeWithBreaks *nodeWithBreaks)
{
	const qReal::Id conditionId = nodeWithBreaks->firstId();

	QList<IntermediateStructurizatorNode *> exitBranches = nodeWithBreaks->exitBranches();
	QList<IntermediateStructurizatorNode *> restBranches = nodeWithBreaks->restBranches();

	switch(semanticsOf(conditionId)) {
	case enums::semantics::conditionalBlock: {
		return createSemanticIfNode(conditionId, exitBranches.first(), nullptr);
	}

	case enums::semantics::switchBlock: {
		QList<IntermediateStructurizatorNode *> allBranches = restBranches + exitBranches;
		return createSemanticSwitchNode(conditionId, allBranches, true);
	}

	case enums::semantics::loopBlock: {
		if (exitBranches.size() != 1 ||
				exitBranches.first()->type() == IntermediateStructurizatorNode::Type::breakNode) {
			break;
		}

		LoopNode *loopNode = mSemanticTree->produceLoop(conditionId);
		loopNode->bodyZone()->appendChild(transformNode(exitBranches.first()));
		return loopNode;
	}

	default:
		break;

	}

	mCantBeGeneratedIntoStructuredCode = true;
	return mSemanticTree->produceSimple();
}

SemanticNode *StructuralControlFlowGenerator::createSemanticIfNode(const Id &conditionId
		, IntermediateStructurizatorNode *thenNode
		, IntermediateStructurizatorNode *elseNode)
{
	IfNode *semanticIf = mSemanticTree->produceConditional(conditionId);
	QPair<LinkInfo, LinkInfo> links = ifBranchesFor(conditionId);

	if (links.first.target != thenNode->firstId()) {
		if (elseNode) {
			IntermediateStructurizatorNode *tmp = thenNode;
			thenNode = elseNode;
			elseNode = tmp;
		} else {
			semanticIf->invertCondition();
		}
	}

	semanticIf->thenZone()->appendChild(transformNode(thenNode));

	if (elseNode) {
		semanticIf->elseZone()->appendChild(transformNode(elseNode));
	}

	return semanticIf;
}

SemanticNode *StructuralControlFlowGenerator::createSemanticSwitchNode(const Id &conditionId
		, const QList<IntermediateStructurizatorNode *> &branches, bool generateIfs)
{
	SwitchNode *semanticSwitch = mSemanticTree->produceSwitch(conditionId);

	QMap<qReal::Id, SemanticNode *> visitedBranch;

	for (const qReal::Id &link : mRepo.outgoingLinks(conditionId)) {
		const QString expression = mRepo.property(link, "Guard").toString();
		const qReal::Id otherVertex = mRepo.otherEntityFromLink(link, conditionId);

		if (visitedBranch.contains(otherVertex)) {
			NonZoneNode * const target = static_cast<NonZoneNode *>(visitedBranch[otherVertex]);
			semanticSwitch->mergeBranch(expression, target);
		} else {
			bool branchNodeWasFound = false;
			SemanticNode *semanticNodeForBranch = nullptr;
			for (IntermediateStructurizatorNode *branchNode : branches) {
				if (branchNode->firstId() == otherVertex) {
					semanticNodeForBranch = transformNode(branchNode);
					branchNodeWasFound = true;
					break;
				}
			}

			if (!branchNodeWasFound) {
				semanticNodeForBranch = mSemanticTree->produceSimple();
			}

			semanticSwitch->addBranch(expression, semanticNodeForBranch);
			visitedBranch[otherVertex] = semanticNodeForBranch;
		}
	}

	if (generateIfs) {
		semanticSwitch->setGenerateIfs();
	}

	return semanticSwitch;
}

void StructuralControlFlowGenerator::appendVertex(const Id &vertex)
{
	mIds.insert(vertex);
	mVerticesNumber++;
	mVertexNumber[vertex] = mVerticesNumber;
}

void StructuralControlFlowGenerator::addEdgeIntoGraph(const Id &from, const Id &to)
{
	int numberFrom = mVertexNumber[from];
	int numberTo = mVertexNumber[to];
	mFollowers[numberFrom].insert(numberTo);
}

void StructuralControlFlowGenerator::appendEdgesAndVertices(const Id &vertex, const QList<LinkInfo> &links)
{
	if (!mIsGraphBeingConstructed) {
		return;
	}

	if (!mIds.contains(vertex)) {
		appendVertex(vertex);
	}

	if (!mStartVertex) {
		mStartVertex = mVertexNumber[vertex];
	}

	for (const LinkInfo &link : links) {
		const qReal::Id otherVertex = link.target;

		if (!mIds.contains(otherVertex)) {
			if (semanticsOf(otherVertex) == enums::semantics::loopBlock) {
				const qReal::Id loopHeader = mSimpleId.sameTypeId();
				mAdditionalVertices.push_back(loopHeader);
				appendVertex(loopHeader);
				appendVertex(otherVertex);
				addEdgeIntoGraph(vertex, loopHeader);
				addEdgeIntoGraph(loopHeader, otherVertex);
				mLoopHeader[mVertexNumber[otherVertex]] = mVertexNumber[loopHeader];
			} else {
				appendVertex(otherVertex);
				addEdgeIntoGraph(vertex, otherVertex);
			}
		} else {
			if (semanticsOf(otherVertex) != enums::semantics::loopBlock
					|| (mLoopNumbers.contains(mVertexNumber[otherVertex])
							&& mVerticesInsideLoopBody.contains(mVertexNumber[vertex]))) {
				addEdgeIntoGraph(vertex, otherVertex);
			} else {
				addEdgeIntoGraph(vertex, mVertexNumber.key(mLoopHeader[mVertexNumber[otherVertex]]));
			}
		}
	}
}

void StructuralControlFlowGenerator::addVerticesInLoopBody(const Id &vertex, const QList<LinkInfo> &links)
{
	if (mVerticesInsideLoopBody.contains(mVertexNumber[vertex])) {
		for (const LinkInfo &link : links) {
			mVerticesInsideLoopBody.insert(mVertexNumber[link.target]);
		}
	}
}

void StructuralControlFlowGenerator::removeVerticesFromLoopBody(const Id &vertex, const QList<LinkInfo> &links)
{
	if (mVerticesInsideLoopBody.contains(mVertexNumber[vertex])) {
		for (const LinkInfo &link : links) {
			mVerticesInsideLoopBody.remove(mVertexNumber[link.target]);
		}
	}
}
