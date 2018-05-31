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
	Q_UNUSED(links)
	mIds.insert(id);
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

	myUtils::IntermediateNode *tree = mStructurizator->performStructurization(&mRepo, mIds);
	if (tree) {
		obtainSemanticTree(tree);
	} else {
		mCantBeGeneratedIntoStructuredCode = true;
	}
}

void StructuralControlFlowGenerator::obtainSemanticTree(myUtils::IntermediateNode *root)
{
	bool hasBreak = root->analyzeBreak();
	SemanticNode * semanticNode = transformNode(root);
	mSemanticTree->setRoot(new RootNode(semanticNode, mSemanticTree));
}

void StructuralControlFlowGenerator::checkAndAppendBlock(ZoneNode *zone, myUtils::IntermediateNode *node)
{
	if (node->type() == myUtils::IntermediateNode::simple) {
		const myUtils::SimpleNode *simpleNode = dynamic_cast<const myUtils::SimpleNode *>(node);

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
SemanticNode *StructuralControlFlowGenerator::transformNode(const myUtils::IntermediateNode *node)
{
	switch (node->type()) {
	case myUtils::IntermediateNode::Type::simple:
		return transformSimple(node);

	case myUtils::IntermediateNode::Type::block:
		return transformBlock(node);

	case myUtils::IntermediateNode::Type::ifThenElseCondition:
		return transformIfThenElse(node);

	case myUtils::IntermediateNode::Type::switchCondition:
		return transformSwitch(node);

	case myUtils::IntermediateNode::Type::infiniteloop:
		return transformSelfLoop(node);

	case myUtils::IntermediateNode::Type::whileloop:
		return transformWhileLoop(node);

	case myUtils::IntermediateNode::Type::breakNode:
		return transformBreakNode();

	case myUtils::IntermediateNode::Type::fakeCycleHead:
		return transformFakeCycleHead();

	case myUtils::IntermediateNode::Type::nodeWithBreaks:
		qDebug() << "Node with breaks must be handled in block or switch or if-then-else";
		mCantBeGeneratedIntoStructuredCode = true;
		return nullptr;

	default:
		qDebug() << "Undefined type of Intermediate node!";
		mCantBeGeneratedIntoStructuredCode = true;

		return nullptr;
	}
}

SemanticNode *StructuralControlFlowGenerator::transformSimple(const myUtils::IntermediateNode *node)
{
	const myUtils::SimpleNode *simpleNode = dynamic_cast<const myUtils::SimpleNode *>(node);
	return mSemanticTree->produceNodeFor(simpleNode->id());
}

SemanticNode *StructuralControlFlowGenerator::transformBlock(const myUtils::IntermediateNode *node)
{
	const myUtils::BlockNode *blockNode = dynamic_cast<const myUtils::BlockNode *>(node);

	if (blockNode->firstNode()->type() == myUtils::IntermediateNode::Type::nodeWithBreaks) {
		myUtils::NodeWithBreaks *nodeWithBreaks = dynamic_cast<myUtils::NodeWithBreaks *>(blockNode->firstNode());
		QList<myUtils::IntermediateNode *> restNodes = {blockNode->secondNode()};
		return createConditionWithBreaks(nodeWithBreaks, restNodes);
	}

	ZoneNode *zone = new ZoneNode(mSemanticTree);

	checkAndAppendBlock(zone, blockNode->firstNode());


	if (blockNode->secondNode()->type() == myUtils::IntermediateNode::Type::nodeWithBreaks) {
		myUtils::NodeWithBreaks *nodeWithBreaks = dynamic_cast<myUtils::NodeWithBreaks *>(blockNode->secondNode());
		QList<myUtils::IntermediateNode *> restNodes = {};

		zone->appendChild(createConditionWithBreaks(nodeWithBreaks, restNodes));
	} else {
		checkAndAppendBlock(zone, blockNode->secondNode());
	}

	return zone;
}

SemanticNode *StructuralControlFlowGenerator::transformIfThenElse(const myUtils::IntermediateNode *node)
{
	const myUtils::IfNode *ifNode = dynamic_cast<const myUtils::IfNode *>(node);

	if (ifNode->condition()->type() == myUtils::IntermediateNode::nodeWithBreaks) {
		myUtils::NodeWithBreaks *nodeWithBreaks = dynamic_cast<myUtils::NodeWithBreaks *>(ifNode->condition());
		QList<myUtils::IntermediateNode *> restBranches = {ifNode->thenBranch(), ifNode->elseBranch()};
		return createConditionWithBreaks(nodeWithBreaks, restBranches);
	}

	const qReal::Id conditionId = ifNode->condition()->firstId();
	const qReal::Id thenId = ifNode->thenBranch()->firstId();

	switch (semanticsOf(conditionId)) {
		case enums::semantics::conditionalBlock: {
			IfNode *semanticIf = new IfNode(conditionId, mSemanticTree);
			QPair<LinkInfo, LinkInfo> links = ifBranchesFor(conditionId);

			if (links.first.target != ifNode->thenBranch()->firstId()) {
				semanticIf->invertCondition();
			}

			semanticIf->thenZone()->appendChild(transformNode(ifNode->thenBranch()));

			if (ifNode->elseBranch()) {
				semanticIf->elseZone()->appendChild(transformNode(ifNode->elseBranch()));
			}

			return semanticIf;
		}

		case enums::semantics::switchBlock: {
			SwitchNode *semanticSwitch = new SwitchNode(conditionId, mSemanticTree);


			//const qReal::Id
			// problem with if-then pattern when "default" branch leads to then
			for (const qReal::Id &link : mRepo.outgoingLinks(conditionId)) {
				const QString expression = mRepo.property(link, "Guard").toString();
				const qReal::Id otherVertex = mRepo.otherEntityFromLink(link, conditionId);
				if (otherVertex == thenId) {
					semanticSwitch->addBranch(expression, transformNode(ifNode->thenBranch()));
				} else {
					semanticSwitch->addBranch(expression, transformNode(ifNode->elseBranch()));
				}
			}

			if (ifNode->hasBreakInside()) {
				semanticSwitch->setGenerateIfs();
			}

			return semanticSwitch;
		}

		case enums::semantics::forkBlock: {
			ForkNode *semanticFork = new ForkNode(conditionId, mSemanticTree);

			if (!ifNode->elseBranch()) {
				qDebug() << "Fork should have all branches";
				mCantBeGeneratedIntoStructuredCode = true;
				return nullptr;
			}

			for (const qReal::Id &link : mRepo.outgoingLinks(conditionId)) {
				const QString expression = mRepo.property(link, "Guard").toString();
				const qReal::Id otherVertex = mRepo.otherEntityFromLink(link, conditionId);
				if (otherVertex == thenId) {
					semanticFork->appendThread(ifNode->thenBranch()->firstId(), expression);
				} else {
					semanticFork->appendThread(ifNode->elseBranch()->firstId(), expression);
				}
			}

			return semanticFork;
		}

		default:
			break;
	}

	qDebug() << "Problem: couldn't transform if-then-else";
	mCantBeGeneratedIntoStructuredCode = true;

	return nullptr;
}

SemanticNode *StructuralControlFlowGenerator::transformSelfLoop(const myUtils::IntermediateNode *node)
{
	const myUtils::SelfLoopNode *selfLoopNode = dynamic_cast<const myUtils::SelfLoopNode *>(node);
	LoopNode *semanticLoop = new LoopNode(qReal::Id(), mSemanticTree);
	semanticLoop->bodyZone()->appendChild(transformNode(selfLoopNode->bodyNode()));
	return semanticLoop;
}

SemanticNode *StructuralControlFlowGenerator::transformWhileLoop(const myUtils::IntermediateNode *node)
{
	const myUtils::WhileNode *whileNode = dynamic_cast<const myUtils::WhileNode *>(node);
	LoopNode *semanticLoop= nullptr;
	if (whileNode->headNode()->type() == myUtils::IntermediateNode::Type::simple
			&& (semanticsOf(whileNode->headNode()->firstId()) == enums::semantics::conditionalBlock ||
				semanticsOf(whileNode->headNode()->firstId()) == enums::semantics::loopBlock)) {
		semanticLoop = new LoopNode(whileNode->headNode()->firstId(), mSemanticTree);
	} else {
		semanticLoop = new LoopNode(qReal::Id(), mSemanticTree);
		semanticLoop->bodyZone()->appendChild(transformNode(whileNode->headNode()));
	}

	semanticLoop->bodyZone()->appendChild(transformNode(whileNode->bodyNode()));

	return semanticLoop;
}

SemanticNode *StructuralControlFlowGenerator::transformSwitch(const myUtils::IntermediateNode *node)
{
	const myUtils::SwitchNode *switchNode = dynamic_cast<const myUtils::SwitchNode *>(node);
	const qReal::Id &conditionId = switchNode->condition()->firstId();
	QList<myUtils::IntermediateNode *> branches = switchNode->branches();

	if (switchNode->condition()->type() == myUtils::IntermediateNode::switchCondition) {
		myUtils::NodeWithBreaks *nodeWithBreaks = dynamic_cast<myUtils::NodeWithBreaks *>(switchNode->condition());
		return createConditionWithBreaks(nodeWithBreaks, branches);
	}

	if (semanticsOf(conditionId) == enums::semantics::switchBlock) {
		SwitchNode *semanticSwitch = new SwitchNode(conditionId, mSemanticTree);

		for (const qReal::Id &link : mRepo.outgoingLinks(conditionId)) {
			const QString expression = mRepo.property(link, "Guard").toString();
			const qReal::Id otherVertex = mRepo.otherEntityFromLink(link, conditionId);

			for (const myUtils::IntermediateNode *branchNode : branches) {
				if (branchNode->firstId() == otherVertex) {
					semanticSwitch->addBranch(expression, transformNode(branchNode));
					break;
				}
			}
		}

		if (switchNode->hasBreakInside()) {
			semanticSwitch->setGenerateIfs();
		}

		return semanticSwitch;

	} else if (semanticsOf(conditionId) == enums::semantics::forkBlock) {
		ForkNode *semanticFork = new ForkNode(conditionId, mSemanticTree);

		for (const qReal::Id &link : mRepo.outgoingLinks(conditionId)) {
			const QString expression = mRepo.property(link, "Expression").toString();
			const qReal::Id otherVertex = mRepo.otherEntityFromLink(link, conditionId);
			semanticFork->appendThread(otherVertex, expression);
		}

		return semanticFork;
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

SemanticNode *StructuralControlFlowGenerator::createConditionWithBreaks(myUtils::NodeWithBreaks *nodeWithBreaks, QList<myUtils::IntermediateNode *> &restNodes)
{
	const qReal::Id conditionId = nodeWithBreaks->firstId();

	QList<myUtils::IntermediateNode *> exitBranches = nodeWithBreaks->exitBranches();

	switch(semanticsOf(conditionId)) {
	case enums::semantics::conditionalBlock: {
		IfNode *semanticIf = new IfNode(conditionId, mSemanticTree);

		const qReal::Id thenId = exitBranches.first()->firstId();
		QPair<LinkInfo, LinkInfo> branchesForIf = ifBranchesFor(conditionId);

		if (branchesForIf.first.target != thenId) {
			semanticIf->invertCondition();
		}

		semanticIf->thenZone()->appendChild(transformNode(exitBranches.first()));

		ZoneNode *zone = new ZoneNode(mSemanticTree);
		zone->appendChild(semanticIf);

		if (!restNodes.isEmpty()) {
			zone->appendChildren({transformNode(restNodes.first()) });
		}

		return zone;
	}

	case enums::semantics::switchBlock: {
		SwitchNode *semanticSwitch = new SwitchNode(conditionId, mSemanticTree);
		QList<myUtils::IntermediateNode *> allBranches = restNodes + exitBranches;


		for (const qReal::Id &link : mRepo.outgoingLinks(conditionId)) {
			const QString expression = mRepo.property(link, "Guard").toString();
			const qReal::Id otherVertex = mRepo.otherEntityFromLink(link, conditionId);

			for (const myUtils::IntermediateNode *branchNode : allBranches) {
				if (branchNode->firstId() == otherVertex) {
					semanticSwitch->addBranch(expression, transformNode(branchNode));
					break;
				}
			}
		}


		semanticSwitch->setGenerateIfs();

		return semanticSwitch;
	}

	default:
		qDebug() << "Problem in createConditionWithBreaks";
		mCantBeGeneratedIntoStructuredCode = false;
		return nullptr;
	}

}
