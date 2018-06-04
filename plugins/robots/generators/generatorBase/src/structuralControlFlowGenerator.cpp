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

#include <QtCore/QDebug>

#include <algorithm>


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
		, bool isThisDiagramMain)
	: ControlFlowGeneratorBase(repo, errorReporter, customizer, validator, diagramId, false, isThisDiagramMain, parent)
	, mStructurizator(new Structurizator(this))
	, mVerticesNumber(0)
{
}

ControlFlowGeneratorBase *StructuralControlFlowGenerator::cloneFor(const Id &diagramId, bool cloneForNewDiagram)
{
	StructuralControlFlowGenerator * const copy = new StructuralControlFlowGenerator(mRepo
			, mErrorReporter, mCustomizer, (cloneForNewDiagram ? *mValidator.clone() : mValidator)
			, diagramId, parent(), false);

	return copy;
}

void StructuralControlFlowGenerator::beforeSearch()
{
}

void StructuralControlFlowGenerator::visit(const Id &id, QList<LinkInfo> &links)
{
	if (mIds.isEmpty()) {
		mStartVertex = id;
	}

	if (!mIds.contains(id)) {
		appendVertex(id);
	}

	ControlFlowGeneratorBase::visit(id, links);

	for (const LinkInfo &link : links) {
		const qReal::Id otherVertex = link.target;

		if (!mIds.contains(otherVertex)) {
			appendVertex(otherVertex);
		}

		addEdgeIntoGraph(id, otherVertex);
	}

}

void StructuralControlFlowGenerator::visitConditional(const Id &id, const QList<LinkInfo> &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
}

void StructuralControlFlowGenerator::visitLoop(const Id &id, const QList<LinkInfo> &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
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

SemanticTree *StructuralControlFlowGenerator::generate(const Id &initialNode, const QString &threadId)
{
	ControlFlowGeneratorBase::generate(initialNode, threadId);
	return mSemanticTree;
}

void StructuralControlFlowGenerator::performGeneration()
{
	//isPerformingGeneration = false;
	mCantBeGeneratedIntoStructuredCode = false;
	ControlFlowGeneratorBase::performGeneration();

	myUtils::IntermediateNode *tree = mStructurizator->performStructurization(mIds, mVertexNumber[mStartVertex], mFollowers, mVertexNumber, mVerticesNumber);

	// add checking whether threads are consistent
	if (tree) {
		//resolveThreads(tree, mThreadId);
		obtainSemanticTree(tree);
	} else {
		mCantBeGeneratedIntoStructuredCode = true;
	}

	if (mCantBeGeneratedIntoStructuredCode) {
		mSemanticTree = nullptr;
	}
}

void StructuralControlFlowGenerator::obtainSemanticTree(myUtils::IntermediateNode *root)
{
	root->analyzeBreak();
	SemanticNode * semanticNode = transformNode(root);
	mSemanticTree->setRoot(new RootNode(semanticNode, mSemanticTree));
}

void StructuralControlFlowGenerator::checkAndAppendBlock(ZoneNode *zone, myUtils::IntermediateNode *node)
{
	if (node->type() == myUtils::IntermediateNode::simple) {
		myUtils::SimpleNode *simpleNode = static_cast<myUtils::SimpleNode *>(node);

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
SemanticNode *StructuralControlFlowGenerator::transformNode(myUtils::IntermediateNode *node)
{
	switch (node->type()) {
	case myUtils::IntermediateNode::Type::simple: {
		myUtils::SimpleNode *simpleNode = static_cast<myUtils::SimpleNode *>(node);
		return transformSimple(simpleNode);
	}

	case myUtils::IntermediateNode::Type::block: {
		myUtils::BlockNode *blockNode = static_cast<myUtils::BlockNode *>(node);
		return transformBlock(blockNode);
	}

	case myUtils::IntermediateNode::Type::ifThenElseCondition: {
		myUtils::IfNode *ifNode = static_cast<myUtils::IfNode *>(node);
		return transformIfThenElse(ifNode);
	}

	case myUtils::IntermediateNode::Type::switchCondition: {
		myUtils::SwitchNode *switchNode = static_cast<myUtils::SwitchNode *>(node);
		return transformSwitch(switchNode);
	}

	case myUtils::IntermediateNode::Type::infiniteloop: {
		myUtils::SelfLoopNode *selfLoopNode = static_cast<myUtils::SelfLoopNode *>(node);
		return transformSelfLoop(selfLoopNode);
	}

	case myUtils::IntermediateNode::Type::whileloop: {
		myUtils::WhileNode *whileNode = static_cast<myUtils::WhileNode *>(node);
		return transformWhileLoop(whileNode);
	}

	case myUtils::IntermediateNode::Type::breakNode: {
		return transformBreakNode();
	}

	case myUtils::IntermediateNode::Type::fakeCycleHead: {
		return transformFakeCycleHead();
	}

	case myUtils::IntermediateNode::Type::nodeWithBreaks: {
		return createConditionWithBreaks(static_cast<myUtils::NodeWithBreaks *>(node));
	}

	default:
		qDebug() << "Undefined type of Intermediate node!";
		mCantBeGeneratedIntoStructuredCode = true;

		return new SimpleNode(qReal::Id(), mSemanticTree);
	}
}

SemanticNode *StructuralControlFlowGenerator::transformSimple(myUtils::SimpleNode *simpleNode)
{
	const qReal::Id id = simpleNode->id();
	SemanticNode *semanticNode = mSemanticTree->produceNodeFor(id);

	if (semanticsOf(id) == enums::semantics::joinBlock) {
		JoinNode *joinSemanticNode = static_cast<JoinNode *>(semanticNode);
		//QString postJoinThreadName = mRepo.property(mRepo.outgoingLinks(id).first(), "Guard").toString();
		joinSemanticNode->setThreadId(mThreadId);
	}

	return semanticNode;
}

SemanticNode *StructuralControlFlowGenerator::transformBlock(myUtils::BlockNode *blockNode)
{
	ZoneNode *zone = new ZoneNode(mSemanticTree);
	checkAndAppendBlock(zone, blockNode->firstNode());
	checkAndAppendBlock(zone, blockNode->secondNode());

	return zone;
}

SemanticNode *StructuralControlFlowGenerator::transformIfThenElse(myUtils::IfNode *ifNode)
{
	if (ifNode->condition()->type() == myUtils::IntermediateNode::nodeWithBreaks) {
		myUtils::NodeWithBreaks *nodeWithBreaks = static_cast<myUtils::NodeWithBreaks *>(ifNode->condition());
		nodeWithBreaks->setRestBranches({ifNode->thenBranch(), ifNode->elseBranch()});
		return createConditionWithBreaks(nodeWithBreaks);
	}

	const qReal::Id conditionId = ifNode->condition()->firstId();

	switch (semanticsOf(conditionId)) {

	case enums::semantics::conditionalBlock: {
		return createSemanticIfNode(conditionId, ifNode->thenBranch(), ifNode->elseBranch());
	}

	case enums::semantics::switchBlock: {
		QList<myUtils::IntermediateNode *> branches = { ifNode->thenBranch() };

		if (ifNode->elseBranch()) {
			branches.append(ifNode->elseBranch());
		}

		return createSemanticSwitchNode(conditionId, branches, ifNode->hasBreakInside());
	}

	default:
		break;
	}

	qDebug() << "Problem: couldn't transform if-then-else";
	mCantBeGeneratedIntoStructuredCode = true;

	return new SimpleNode(qReal::Id(), mSemanticTree);
}

SemanticNode *StructuralControlFlowGenerator::transformSelfLoop(myUtils::SelfLoopNode *selfLoopNode)
{
	LoopNode *semanticLoop = new LoopNode(qReal::Id(), mSemanticTree);
	semanticLoop->bodyZone()->appendChild(transformNode(selfLoopNode->bodyNode()));
	return semanticLoop;
}

SemanticNode *StructuralControlFlowGenerator::transformWhileLoop(myUtils::WhileNode *whileNode)
{
	myUtils::IntermediateNode *headNode = whileNode->headNode();
	myUtils::IntermediateNode *bodyNode = whileNode->bodyNode();
	myUtils::IntermediateNode *exitNode = whileNode->exitNode();

	LoopNode *semanticLoop = nullptr;
	const qReal::Id conditionId = headNode->firstId();

	if (headNode->type() == myUtils::IntermediateNode::Type::simple) {
		switch(semanticsOf(conditionId)) {
		case enums::semantics::conditionalBlock:
		case enums::semantics::loopBlock: {
			semanticLoop = new LoopNode(conditionId, mSemanticTree);
			semanticLoop->bodyZone()->appendChild(transformNode(bodyNode));
			return semanticLoop;
		}

		case enums::semantics::switchBlock: {
			QList<myUtils::IntermediateNode *> exitBranches;
			exitBranches.append(new myUtils::BreakNode(exitNode->firstId(), mStructurizator));

			myUtils::NodeWithBreaks *nodeWithBreaks = new myUtils::NodeWithBreaks(headNode, exitBranches, mStructurizator);
			nodeWithBreaks->setRestBranches( { bodyNode } );

			semanticLoop = new LoopNode(qReal::Id(), mSemanticTree);
			semanticLoop->bodyZone()->appendChild(createConditionWithBreaks(nodeWithBreaks));
			return semanticLoop;
		}

		default:
			break;
		}
	}

	semanticLoop = new LoopNode(qReal::Id(), mSemanticTree);
	semanticLoop->bodyZone()->appendChild(transformNode(headNode));
	semanticLoop->bodyZone()->appendChild(transformNode(bodyNode));
	return semanticLoop;
}

SemanticNode *StructuralControlFlowGenerator::transformSwitch(myUtils::SwitchNode *switchNode)
{
	const qReal::Id &conditionId = switchNode->condition()->firstId();
	QList<myUtils::IntermediateNode *> branches = switchNode->branches();

	if (switchNode->condition()->type() == myUtils::IntermediateNode::nodeWithBreaks) {
		myUtils::NodeWithBreaks *nodeWithBreaks = static_cast<myUtils::NodeWithBreaks *>(switchNode->condition());
		nodeWithBreaks->setRestBranches(branches);
		return createConditionWithBreaks(nodeWithBreaks);
	}

	if (semanticsOf(conditionId) == enums::semantics::switchBlock) {
		return createSemanticSwitchNode(conditionId, branches, switchNode->hasBreakInside());
	}

	qDebug() << "Problem: couldn't identidy semantics id for switchNode";
	mCantBeGeneratedIntoStructuredCode = true;

	return new SimpleNode(qReal::Id(), mSemanticTree);
}

SemanticNode *StructuralControlFlowGenerator::transformBreakNode()
{
	return semantics::SimpleNode::createBreakNode(mSemanticTree);
}

SemanticNode *StructuralControlFlowGenerator::transformFakeCycleHead()
{
	return new SimpleNode(qReal::Id(), mSemanticTree);
}

SemanticNode *StructuralControlFlowGenerator::createConditionWithBreaks(myUtils::NodeWithBreaks *nodeWithBreaks)
{
	const qReal::Id conditionId = nodeWithBreaks->firstId();

	QList<myUtils::IntermediateNode *> exitBranches = nodeWithBreaks->exitBranches();
	QList<myUtils::IntermediateNode *> restBranches = nodeWithBreaks->restBranches();

	switch(semanticsOf(conditionId)) {
	case enums::semantics::conditionalBlock: {
		return createSemanticIfNode(conditionId, exitBranches.first(), nullptr);
	}

	case enums::semantics::switchBlock: {
		QList<myUtils::IntermediateNode *> allBranches = restBranches + exitBranches;
		return createSemanticSwitchNode(conditionId, allBranches, true);
	}

	default:
		qDebug() << "Problem in createConditionWithBreaks";
		mCantBeGeneratedIntoStructuredCode = false;
		return new SimpleNode(qReal::Id(), mSemanticTree);
	}

}

SemanticNode *StructuralControlFlowGenerator::createSemanticIfNode(const Id &conditionId, myUtils::IntermediateNode *thenNode, myUtils::IntermediateNode *elseNode)
{
	IfNode *semanticIf = new IfNode(conditionId, mSemanticTree);
	QPair<LinkInfo, LinkInfo> links = ifBranchesFor(conditionId);

	if (links.first.target != thenNode->firstId()) {
		semanticIf->invertCondition();
	}

	semanticIf->thenZone()->appendChild(transformNode(thenNode));

	if (elseNode) {
		semanticIf->elseZone()->appendChild(transformNode(elseNode));
	}

	return semanticIf;
}

SemanticNode *StructuralControlFlowGenerator::createSemanticSwitchNode(const Id &conditionId, const QList<myUtils::IntermediateNode *> &branches, bool generateIfs)
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
			for (myUtils::IntermediateNode *branchNode : branches) {
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
	mFollowers[mVertexNumber[from]].insert(mVertexNumber[to]);
}
