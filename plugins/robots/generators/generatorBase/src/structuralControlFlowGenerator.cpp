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
	: ControlFlowGeneratorBase(repo, errorReporter, customizer, validator, diagramId, parent, isThisDiagramMain)
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

	if (mCustomizer.isSubprogramCall(id)) {
		mCustomizer.factory()->subprograms()->usageFound(id);
	} else if (mCustomizer.semanticsOf(id) == enums::semantics::forkBlock) {

		QList<LinkInfo> newLinks = {};
		for (const LinkInfo &link : links) {
			QString threadName = mRepo.property(link.linkId, "Guard").toString();
			if (threadName != mThreadId) {
				mCustomizer.factory()->threads().registerThread(link.target, threadName);
			} else {
				newLinks.append(link);
			}
		}

		links = newLinks;

	} else if (mCustomizer.semanticsOf(id) == enums::semantics::joinBlock) {
		QString threadNameAfterJoin = mRepo.property(links.first().linkId, "Guard").toString();

		if (threadNameAfterJoin != mThreadId) {
			links.clear();
			mCustomizer.factory()->threads().addJoin(id, mThreadId);
		}
	}

	for (const LinkInfo &link : links) {
		const qReal::Id otherVertex = link.target;

		if (!mIds.contains(otherVertex)) {
			appendVertex(otherVertex);
		}

		int v = mVertexNumber[id];
		int u = mVertexNumber[otherVertex];
		mFollowers[v].insert(u);
	}

}

void StructuralControlFlowGenerator::visitRegular(const Id &id, const QList<LinkInfo> &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
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

		return nullptr;
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
	} else if (semanticsOf(id) == enums::semantics::forkBlock) {
		ForkNode *forkSemanticNode = static_cast<ForkNode *>(semanticNode);
		for (const qReal::Id &link : mRepo.outgoingLinks(id)) {
			qReal::Id anotherId = mRepo.otherEntityFromLink(link, id);
			QString threadName = mRepo.property(link, "Guard").toString();

			if (threadName != mThreadId) {
				forkSemanticNode->appendThread(anotherId, threadName);
			}
		}
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
		QList<myUtils::IntermediateNode *> branches = {ifNode->thenBranch()};

		if (ifNode->elseBranch()) {
			branches.append(ifNode->elseBranch());
		}

		return createSemanticSwitchNode(conditionId, branches, ifNode->hasBreakInside());
	}

//	case enums::semantics::forkBlock: {

//		QList<myUtils::IntermediateNode *> branches = { ifNode->thenBranch()};
//		if (ifNode->elseBranch()) {
//			branches.append(ifNode->elseBranch());
//		}

//		addThreadsToJoin(branches, ifNode->exit());
//		return createSemanticForkNode(conditionId, branches, ifNode->currentThread());
//	}

	default:
		break;
	}

	qDebug() << "Problem: couldn't transform if-then-else";
	mCantBeGeneratedIntoStructuredCode = true;

	return nullptr;
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
	} else if (semanticsOf(conditionId) == enums::semantics::forkBlock) {

		if (switchNode->exit()) {
			addThreadsToJoin(branches, switchNode->exit());
		}

		return createSemanticForkNode(conditionId, branches, switchNode->currentThread());
	}

	qDebug() << "Problem: couldn't identidy semantics id for switchNode";
	mCantBeGeneratedIntoStructuredCode = true;

	return nullptr;
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
		return nullptr;
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

SemanticNode *StructuralControlFlowGenerator::createSemanticForkNode(const Id &conditionId, const QList<myUtils::IntermediateNode *> &branches
																		, const QString &currentThreadExpression)
{
	ForkNode *semanticFork = new ForkNode(conditionId, mSemanticTree);
	SemanticNode *currentThreadNode = nullptr;

	for (myUtils::IntermediateNode *branch : branches) {
		if (branch->currentThread() == currentThreadExpression) {
			currentThreadNode = transformNode(branch);
			break;
		}
	}

	if (!currentThreadNode) {
		currentThreadNode = new SimpleNode(qReal::Id(), mSemanticTree);
	}

	ZoneNode *zone = new ZoneNode(mSemanticTree);
	zone->appendChild(semanticFork);
	zone->appendChild(currentThreadNode);

	for (const qReal::Id &link : mRepo.outgoingLinks(conditionId)) {
		const QString expression = mRepo.property(link, "Guard").toString();
		const qReal::Id anotherThreadFirstVertex = mRepo.otherEntityFromLink(link, conditionId);

		if (expression != currentThreadExpression) {
			mCustomizer.factory()->threads().registerThread(anotherThreadFirstVertex, expression);

			semanticFork->appendThread(anotherThreadFirstVertex, expression);

			SemanticNode *newRoot = nullptr;
			for (myUtils::IntermediateNode *branch : branches) {
				if (branch->firstId() == anotherThreadFirstVertex) {
					newRoot = transformNode(branch);
					break;
				}
			}

			if (!newRoot) {
				newRoot = new SimpleNode(qReal::Id(), mSemanticTree);
			}

			semantics::SemanticTree *newTree = new semantics::SemanticTree(mCustomizer, anotherThreadFirstVertex, false, mSemanticTree->parent());
			newTree->setRoot(new RootNode(newRoot, newTree));
			mCustomizer.factory()->threads().threadProcessed(anotherThreadFirstVertex, *newTree);
		}
	}

	return zone;
}

void StructuralControlFlowGenerator::addThreadsToJoin(const QList<myUtils::IntermediateNode *> &branches, myUtils::IntermediateNode *joinNode)
{
	const qReal::Id joinId = joinNode->firstId();

	// mistake. not all branches should be joined
	for (const myUtils::IntermediateNode *branch : branches) {
		const qReal::Id firstId = branch->firstId();
		if (firstId.isNull()) {
			continue;
		}

		const QString threadId = mRepo.property(mRepo.incomingLinks(firstId).first(), "Guard").toString();
		if (threadId != joinNode->currentThread()) {
			mCustomizer.factory()->threads().addJoin(joinId, threadId);
		}
	}
}

void StructuralControlFlowGenerator::resolveThreads(myUtils::IntermediateNode *node, const QString &currentThreadName)
{
	node->setCurrentThread(currentThreadName);

	if (semanticsOf(node->firstId()) == enums::semantics::forkBlock
			&& (node->type() == myUtils::IntermediateNode::switchCondition
				|| node->type() == myUtils::IntermediateNode::ifThenElseCondition
				|| node->type() == myUtils::IntermediateNode::ifThenCondition)) {
		for (myUtils::IntermediateNode *child : node->childrenNodes()) {
			const qReal::Id firstIdOfAnotherThread = child->firstId();
			QString otherThreadName = currentThreadName;

			if (!firstIdOfAnotherThread.isNull()) {
				otherThreadName = mRepo.property(mRepo.incomingLinks(firstIdOfAnotherThread).first(), "Guard").toString();
			}

			resolveThreads(child, otherThreadName);
		}
	}
//	else if (semanticsOf(node->firstId()) == enums::semantics::joinBlock) {
//		// newThreadName = mRepo.mRepo.outgoingLinks()
//	}
	else {
		for (myUtils::IntermediateNode *child : node->childrenNodes()) {
			resolveThreads(child, currentThreadName);
		}
	}

}

void StructuralControlFlowGenerator::appendVertex(const Id &vertex)
{
	mIds.insert(vertex);
	mVerticesNumber++;
	mVertexNumber[vertex] = mVerticesNumber;
}
