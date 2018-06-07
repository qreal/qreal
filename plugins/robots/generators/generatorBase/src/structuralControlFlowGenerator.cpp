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
		, bool isThisDiagramMain)
	: ControlFlowGeneratorBase(repo, errorReporter, customizer, validator, diagramId, parent, isThisDiagramMain)
	, mCantBeGeneratedIntoStructuredCode(false)
	, mStructurizator(new Structurizator(this))
	, mVerticesNumber(0)
	, mStartVertex(0)
	, mIsGraphBeingConstructed(true)
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
	if (mIsGraphBeingConstructed) {
		if (!mIds.contains(id)) {
			appendVertex(id);
		}

		if (!mStartVertex) {
			mStartVertex = mVertexNumber[id];
		}

		appendEdges(id, links);

		if (mVerticesInsideLoopBody.contains(mVertexNumber[id])) {
			for (const LinkInfo &link : links) {
				mVerticesInsideLoopBody.insert(mVertexNumber[link.target]);
			}
		}
	}

	ControlFlowGeneratorBase::visit(id, links);
}

void StructuralControlFlowGenerator::afterVisit(const Id &id, QList<LinkInfo> &links)
{
	if (!mIsGraphBeingConstructed) {
		return;
	}

	if (semanticsOf(id) == enums::semantics::loopBlock) {
		mLoopNumbers.pop();

		QPair<LinkInfo, LinkInfo> loopBranches = loopBranchesFor(id);
		mVerticesInsideLoopBody.insert(mVertexNumber[loopBranches.first.target]);
	}

	if (mVerticesInsideLoopBody.contains(mVertexNumber[id])) {
		for (const LinkInfo &link : links) {
			mVerticesInsideLoopBody.remove(mVertexNumber[link.target]);
		}
	}
}

void StructuralControlFlowGenerator::visitConditional(const Id &id, const QList<LinkInfo> &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
}

void StructuralControlFlowGenerator::visitLoop(const Id &id, const QList<LinkInfo> &links)
{
	Q_UNUSED(links)

	if (!mIsGraphBeingConstructed) {
		return;
	}

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
	myUtils::IntermediateNode *tree = mStructurizator->performStructurization(mIds, mStartVertex, mFollowers, mVertexNumber, mVerticesNumber);

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
	return mSemanticTree->produceNodeFor(simpleNode->id());
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

	case enums::semantics::loopBlock: {
		if (ifNode->elseBranch() && ifNode->exit()->firstId() == ifNode->firstId()) {
			ZoneNode *zone = new ZoneNode(mSemanticTree);
			const qReal::Id loopCondition = ifNode->condition()->firstId();
			LoopNode *innerLoop = new LoopNode(loopCondition, mSemanticTree);

			QPair<LinkInfo, LinkInfo> loopBranches = loopBranchesFor(loopCondition);
			myUtils::IntermediateNode *restBranch = ifNode->thenBranch();

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
		if (elseNode) {
			myUtils::IntermediateNode *tmp = thenNode;
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

void StructuralControlFlowGenerator::appendEdges(const Id &vertex, QList<LinkInfo> &links)
{
	for (const LinkInfo &link : links) {
		const qReal::Id otherVertex = link.target;

		if (!mIds.contains(otherVertex)) {
			if (semanticsOf(otherVertex) == enums::semantics::loopBlock) {
				const qReal::Id loopHeader = qReal::Id();
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
