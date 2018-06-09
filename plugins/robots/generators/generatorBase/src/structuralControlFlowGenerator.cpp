/* Copyright 2017 QReal Research Group
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

#include <algorithm>
#include <QStack>


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
	structurizatorNodes::IntermediateNode *tree = mStructurizator->performStructurization(mIds, mStartVertex, mFollowers, mVertexNumber, mVerticesNumber);

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

bool StructuralControlFlowGenerator::applyRuleWhileVisiting(SemanticTransformationRule * const rule)
{
	if (!mIsGraphBeingConstructed) {
		return ControlFlowGeneratorBase::applyRuleWhileVisiting(rule);
	}

	return false;
}

void StructuralControlFlowGenerator::obtainSemanticTree(structurizatorNodes::IntermediateNode *root)
{
	root->analyzeBreak();
	SemanticNode * semanticNode = transformNode(root);
	mSemanticTree->setRoot(new RootNode(semanticNode, mSemanticTree));
}

void StructuralControlFlowGenerator::checkAndAppendBlock(ZoneNode *zone, structurizatorNodes::IntermediateNode *node)
{
	if (node->type() == structurizatorNodes::IntermediateNode::simple) {
		structurizatorNodes::SimpleNode *simpleNode = static_cast<structurizatorNodes::SimpleNode *>(node);

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
SemanticNode *StructuralControlFlowGenerator::transformNode(structurizatorNodes::IntermediateNode *node)
{
	switch (node->type()) {
	case structurizatorNodes::IntermediateNode::Type::simple: {
		structurizatorNodes::SimpleNode *simpleNode = static_cast<structurizatorNodes::SimpleNode *>(node);
		return transformSimple(simpleNode);
	}

	case structurizatorNodes::IntermediateNode::Type::block: {
		structurizatorNodes::BlockNode *blockNode = static_cast<structurizatorNodes::BlockNode *>(node);
		return transformBlock(blockNode);
	}

	case structurizatorNodes::IntermediateNode::Type::ifThenElseCondition: {
		structurizatorNodes::IfNode *ifNode = static_cast<structurizatorNodes::IfNode *>(node);
		return transformIfThenElse(ifNode);
	}

	case structurizatorNodes::IntermediateNode::Type::switchCondition: {
		structurizatorNodes::SwitchNode *switchNode = static_cast<structurizatorNodes::SwitchNode *>(node);
		return transformSwitch(switchNode);
	}

	case structurizatorNodes::IntermediateNode::Type::infiniteloop: {
		structurizatorNodes::SelfLoopNode *selfLoopNode = static_cast<structurizatorNodes::SelfLoopNode *>(node);
		return transformSelfLoop(selfLoopNode);
	}

	case structurizatorNodes::IntermediateNode::Type::whileloop: {
		structurizatorNodes::WhileNode *whileNode = static_cast<structurizatorNodes::WhileNode *>(node);
		return transformWhileLoop(whileNode);
	}

	case structurizatorNodes::IntermediateNode::Type::breakNode: {
		return transformBreakNode();
	}

	case structurizatorNodes::IntermediateNode::Type::nodeWithBreaks: {
		return createConditionWithBreaks(static_cast<structurizatorNodes::NodeWithBreaks *>(node));
	}

	default:
		mCantBeGeneratedIntoStructuredCode = true;

		return new SimpleNode(qReal::Id(), mSemanticTree);
	}
}

SemanticNode *StructuralControlFlowGenerator::transformSimple(structurizatorNodes::SimpleNode *simpleNode)
{
	return mSemanticTree->produceNodeFor(simpleNode->id());
}

SemanticNode *StructuralControlFlowGenerator::transformBlock(structurizatorNodes::BlockNode *blockNode)
{
	ZoneNode *zone = new ZoneNode(mSemanticTree);
	checkAndAppendBlock(zone, blockNode->firstNode());
	checkAndAppendBlock(zone, blockNode->secondNode());

	return zone;
}

SemanticNode *StructuralControlFlowGenerator::transformIfThenElse(structurizatorNodes::IfNode *ifNode)
{
	if (ifNode->condition()->type() == structurizatorNodes::IntermediateNode::nodeWithBreaks) {
		structurizatorNodes::NodeWithBreaks *nodeWithBreaks = static_cast<structurizatorNodes::NodeWithBreaks *>(ifNode->condition());
		nodeWithBreaks->setRestBranches({ifNode->thenBranch(), ifNode->elseBranch()});
		return createConditionWithBreaks(nodeWithBreaks);
	}

	const qReal::Id conditionId = ifNode->condition()->firstId();

	switch (semanticsOf(conditionId)) {

	case enums::semantics::conditionalBlock: {
		return createSemanticIfNode(conditionId, ifNode->thenBranch(), ifNode->elseBranch());
	}

	case enums::semantics::switchBlock: {
		QList<structurizatorNodes::IntermediateNode *> branches = { ifNode->thenBranch() };

		if (ifNode->elseBranch()) {
			branches.append(ifNode->elseBranch());
		}

		return createSemanticSwitchNode(conditionId, branches, ifNode->hasBreakInside());
	}

	case enums::semantics::loopBlock: {
		if ((ifNode->exit() && ifNode->elseBranch() && ifNode->exit()->firstId() == ifNode->firstId()) || (!ifNode->exit() && ifNode->elseBranch())) {
			ZoneNode *zone = new ZoneNode(mSemanticTree);
			const qReal::Id loopCondition = ifNode->condition()->firstId();
			LoopNode *innerLoop = new LoopNode(loopCondition, mSemanticTree);

			QPair<LinkInfo, LinkInfo> loopBranches = loopBranchesFor(loopCondition);
			structurizatorNodes::IntermediateNode *restBranch = ifNode->thenBranch();

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

	return new SimpleNode(qReal::Id(), mSemanticTree);
}

SemanticNode *StructuralControlFlowGenerator::transformSelfLoop(structurizatorNodes::SelfLoopNode *selfLoopNode)
{
	LoopNode *semanticLoop = new LoopNode(qReal::Id(), mSemanticTree);
	semanticLoop->bodyZone()->appendChild(transformNode(selfLoopNode->bodyNode()));
	return semanticLoop;
}

SemanticNode *StructuralControlFlowGenerator::transformWhileLoop(structurizatorNodes::WhileNode *whileNode)
{
	structurizatorNodes::IntermediateNode *headNode = whileNode->headNode();
	structurizatorNodes::IntermediateNode *bodyNode = whileNode->bodyNode();
	structurizatorNodes::IntermediateNode *exitNode = whileNode->exitNode();

	LoopNode *semanticLoop = nullptr;
	const qReal::Id conditionId = headNode->firstId();

	if (headNode->type() == structurizatorNodes::IntermediateNode::Type::simple) {
		switch (semanticsOf(conditionId)) {
		case enums::semantics::conditionalBlock: {
			if (ifBranchesFor(conditionId).first.target == bodyNode->firstId()) {
				semanticLoop = new LoopNode(conditionId, mSemanticTree);
			} else {
				semanticLoop = new LoopNode(qReal::Id(), mSemanticTree);
				IfNode *conditionNode = new IfNode(conditionId, mSemanticTree);
				conditionNode->thenZone()->appendChild(SimpleNode::createBreakNode(mSemanticTree));
				semanticLoop->bodyZone()->appendChild(conditionNode);
			}

			semanticLoop->bodyZone()->appendChild(transformNode(bodyNode));
			return semanticLoop;
		}

		case enums::semantics::loopBlock: {
			semanticLoop = new LoopNode(conditionId, mSemanticTree);
			semanticLoop->bodyZone()->appendChild(transformNode(bodyNode));
			return semanticLoop;
		}

		case enums::semantics::switchBlock: {
			QList<structurizatorNodes::IntermediateNode *> exitBranches;
			exitBranches.append(new structurizatorNodes::BreakNode(exitNode->firstId(), mStructurizator));

			structurizatorNodes::NodeWithBreaks *nodeWithBreaks = new structurizatorNodes::NodeWithBreaks(headNode, exitBranches, mStructurizator);
			nodeWithBreaks->setRestBranches( { bodyNode } );

			semanticLoop = new LoopNode(qReal::Id(), mSemanticTree);
			semanticLoop->bodyZone()->appendChild(createConditionWithBreaks(nodeWithBreaks));
			return semanticLoop;
		}

		default:
			break;
		}
	} else if (headNode->type() == structurizatorNodes::IntermediateNode::Type::nodeWithBreaks
					&& semanticsOf(conditionId) == enums::semantics::loopBlock) {
		structurizatorNodes::NodeWithBreaks *nodeWitBreaks = static_cast<structurizatorNodes::NodeWithBreaks *>(headNode);
		if (nodeWitBreaks->exitBranches().size() != 1 || nodeWitBreaks->exitBranches().first()->type() == structurizatorNodes::IntermediateNode::block) {
			mCantBeGeneratedIntoStructuredCode = true;
			return new SimpleNode(qReal::Id(), mSemanticTree);
		} else {
			semanticLoop = new LoopNode(conditionId, mSemanticTree);
			semanticLoop->bodyZone()->appendChild(transformNode(bodyNode));
			return semanticLoop;
		}
	}

	semanticLoop = new LoopNode(qReal::Id(), mSemanticTree);
	semanticLoop->bodyZone()->appendChild(transformNode(headNode));
	semanticLoop->bodyZone()->appendChild(transformNode(bodyNode));
	return semanticLoop;
}

SemanticNode *StructuralControlFlowGenerator::transformSwitch(structurizatorNodes::SwitchNode *switchNode)
{
	const qReal::Id &conditionId = switchNode->condition()->firstId();
	QList<structurizatorNodes::IntermediateNode *> branches = switchNode->branches();

	if (switchNode->condition()->type() == structurizatorNodes::IntermediateNode::nodeWithBreaks) {
		structurizatorNodes::NodeWithBreaks *nodeWithBreaks = static_cast<structurizatorNodes::NodeWithBreaks *>(switchNode->condition());
		nodeWithBreaks->setRestBranches(branches);
		return createConditionWithBreaks(nodeWithBreaks);
	}

	if (semanticsOf(conditionId) == enums::semantics::switchBlock) {
		return createSemanticSwitchNode(conditionId, branches, switchNode->hasBreakInside());
	}

	mCantBeGeneratedIntoStructuredCode = true;

	return new SimpleNode(qReal::Id(), mSemanticTree);
}

SemanticNode *StructuralControlFlowGenerator::transformBreakNode()
{
	return semantics::SimpleNode::createBreakNode(mSemanticTree);
}

SemanticNode *StructuralControlFlowGenerator::createConditionWithBreaks(structurizatorNodes::NodeWithBreaks *nodeWithBreaks)
{
	const qReal::Id conditionId = nodeWithBreaks->firstId();

	QList<structurizatorNodes::IntermediateNode *> exitBranches = nodeWithBreaks->exitBranches();
	QList<structurizatorNodes::IntermediateNode *> restBranches = nodeWithBreaks->restBranches();

	switch(semanticsOf(conditionId)) {
	case enums::semantics::conditionalBlock: {
		return createSemanticIfNode(conditionId, exitBranches.first(), nullptr);
	}

	case enums::semantics::switchBlock: {
		QList<structurizatorNodes::IntermediateNode *> allBranches = restBranches + exitBranches;
		return createSemanticSwitchNode(conditionId, allBranches, true);
	}

	case enums::semantics::loopBlock: {
		if (exitBranches.size() != 1 || exitBranches.first()->type() == structurizatorNodes::IntermediateNode::Type::breakNode) {
			break;
		}

		LoopNode *loopNode = new LoopNode(conditionId, mSemanticTree);
		loopNode->bodyZone()->appendChild(transformNode(exitBranches.first()));
		return loopNode;
	}

	default:
		break;

	}

	mCantBeGeneratedIntoStructuredCode = true;
	return new SimpleNode(qReal::Id(), mSemanticTree);
}

SemanticNode *StructuralControlFlowGenerator::createSemanticIfNode(const Id &conditionId, structurizatorNodes::IntermediateNode *thenNode, structurizatorNodes::IntermediateNode *elseNode)
{
	IfNode *semanticIf = new IfNode(conditionId, mSemanticTree);
	QPair<LinkInfo, LinkInfo> links = ifBranchesFor(conditionId);

	if (links.first.target != thenNode->firstId()) {
		if (elseNode) {
			structurizatorNodes::IntermediateNode *tmp = thenNode;
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

SemanticNode *StructuralControlFlowGenerator::createSemanticSwitchNode(const Id &conditionId, const QList<structurizatorNodes::IntermediateNode *> &branches, bool generateIfs)
{
	SwitchNode *semanticSwitch = new SwitchNode(conditionId, mSemanticTree);

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
			for (structurizatorNodes::IntermediateNode *branchNode : branches) {
				if (branchNode->firstId() == otherVertex) {
					semanticNodeForBranch = transformNode(branchNode);
					branchNodeWasFound = true;
					break;
				}
			}

			if (!branchNodeWasFound) {
				semanticNodeForBranch = new SimpleNode(qReal::Id(), this);
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
			if (semanticsOf(otherVertex) != enums::semantics::loopBlock ||
					(mLoopNumbers.contains(mVertexNumber[otherVertex]) && mVerticesInsideLoopBody.contains(mVertexNumber[vertex]))) {
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
