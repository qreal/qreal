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

	if (mCantBeGeneratedIntoStructuredCode) {
		mSemanticTree = nullptr;
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
		myUtils::SimpleNode *simpleNode = dynamic_cast<myUtils::SimpleNode *>(node);

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

	case myUtils::IntermediateNode::Type::whileloop: {
		myUtils::WhileNode *whileNode = dynamic_cast<myUtils::WhileNode *>(node);
		return transformWhileLoop(whileNode);
	}

	case myUtils::IntermediateNode::Type::breakNode:
		return transformBreakNode();

	case myUtils::IntermediateNode::Type::fakeCycleHead:
		return transformFakeCycleHead();

	case myUtils::IntermediateNode::Type::nodeWithBreaks:
		return createConditionWithBreaks(dynamic_cast<myUtils::NodeWithBreaks *>(node));

	default:
		qDebug() << "Undefined type of Intermediate node!";
		mCantBeGeneratedIntoStructuredCode = true;

		return nullptr;
	}
}

SemanticNode *StructuralControlFlowGenerator::transformSimple(myUtils::IntermediateNode *node)
{
	myUtils::SimpleNode *simpleNode = dynamic_cast<myUtils::SimpleNode *>(node);
	return mSemanticTree->produceNodeFor(simpleNode->id());
}

SemanticNode *StructuralControlFlowGenerator::transformBlock(myUtils::IntermediateNode *node)
{
	myUtils::BlockNode *blockNode = dynamic_cast<myUtils::BlockNode *>(node);

	ZoneNode *zone = new ZoneNode(mSemanticTree);
	checkAndAppendBlock(zone, blockNode->firstNode());
	checkAndAppendBlock(zone, blockNode->secondNode());

	return zone;
}

SemanticNode *StructuralControlFlowGenerator::transformIfThenElse(myUtils::IntermediateNode *node)
{
	myUtils::IfNode *ifNode = dynamic_cast<myUtils::IfNode *>(node);

	if (ifNode->condition()->type() == myUtils::IntermediateNode::nodeWithBreaks) {
		myUtils::NodeWithBreaks *nodeWithBreaks = dynamic_cast<myUtils::NodeWithBreaks *>(ifNode->condition());
		nodeWithBreaks->setRestBranches({ifNode->thenBranch(), ifNode->elseBranch()});
		return createConditionWithBreaks(nodeWithBreaks);
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

SemanticNode *StructuralControlFlowGenerator::transformSelfLoop(myUtils::IntermediateNode *node)
{
	myUtils::SelfLoopNode *selfLoopNode = dynamic_cast<myUtils::SelfLoopNode *>(node);
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

	if (headNode->type() == myUtils::IntermediateNode::Type::simple
			&& (semanticsOf(conditionId) == enums::semantics::conditionalBlock ||
				semanticsOf(conditionId) == enums::semantics::loopBlock)) {
		semanticLoop = new LoopNode(conditionId, mSemanticTree);
	} else if (semanticsOf(conditionId) == enums::semantics::switchBlock) {

		QList<myUtils::IntermediateNode *> exitBranches;
		exitBranches.append(new myUtils::BreakNode(exitNode->firstId(), mStructurizator));

		myUtils::NodeWithBreaks *nodeWithBreaks = new myUtils::NodeWithBreaks(headNode, exitBranches, mStructurizator);
		nodeWithBreaks->setRestBranches( {whileNode->bodyNode() } );

		semanticLoop = new LoopNode(qReal::Id(), mSemanticTree);
		semanticLoop->bodyZone()->appendChild(createConditionWithBreaks(nodeWithBreaks));
		return semanticLoop;
	} else {
		semanticLoop = new LoopNode(qReal::Id(), mSemanticTree);
		semanticLoop->bodyZone()->appendChild(transformNode(whileNode->headNode()));
	}

	semanticLoop->bodyZone()->appendChild(transformNode(whileNode->bodyNode()));

	return semanticLoop;
}

SemanticNode *StructuralControlFlowGenerator::transformSwitch(myUtils::IntermediateNode *node)
{
	myUtils::SwitchNode *switchNode = dynamic_cast<myUtils::SwitchNode *>(node);
	const qReal::Id &conditionId = switchNode->condition()->firstId();
	QList<myUtils::IntermediateNode *> branches = switchNode->branches();

	if (switchNode->condition()->type() == myUtils::IntermediateNode::switchCondition) {
		myUtils::NodeWithBreaks *nodeWithBreaks = dynamic_cast<myUtils::NodeWithBreaks *>(switchNode->condition());
		nodeWithBreaks->setRestBranches(branches);
		return createConditionWithBreaks(nodeWithBreaks);
	}

	if (semanticsOf(conditionId) == enums::semantics::switchBlock) {
		SwitchNode *semanticSwitch = new SwitchNode(conditionId, mSemanticTree);

		for (const qReal::Id &link : mRepo.outgoingLinks(conditionId)) {
			const QString expression = mRepo.property(link, "Guard").toString();
			const qReal::Id otherVertex = mRepo.otherEntityFromLink(link, conditionId);

			for (myUtils::IntermediateNode *branchNode : branches) {
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

SemanticNode *StructuralControlFlowGenerator::createConditionWithBreaks(myUtils::NodeWithBreaks *nodeWithBreaks)
{
	const qReal::Id conditionId = nodeWithBreaks->firstId();

	QList<myUtils::IntermediateNode *> exitBranches = nodeWithBreaks->exitBranches();
	QList<myUtils::IntermediateNode *> restBranches = nodeWithBreaks->restBranches();

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

		if (!restBranches.isEmpty()) {
			zone->appendChildren({transformNode(restBranches.first()) });
		}

		return zone;
	}

	case enums::semantics::switchBlock: {
		SwitchNode *semanticSwitch = new SwitchNode(conditionId, mSemanticTree);
		QList<myUtils::IntermediateNode *> allBranches = restBranches + exitBranches;


		for (const qReal::Id &link : mRepo.outgoingLinks(conditionId)) {
			const QString expression = mRepo.property(link, "Guard").toString();
			const qReal::Id otherVertex = mRepo.otherEntityFromLink(link, conditionId);

			bool branchNodeWasFound = false;
			for (myUtils::IntermediateNode *branchNode : allBranches) {
				if (branchNode->firstId() == otherVertex) {
					semanticSwitch->addBranch(expression, transformNode(branchNode));
					branchNodeWasFound = true;
					break;
				}
			}

			if (!branchNodeWasFound) {
				semanticSwitch->addBranch(expression, new SimpleNode(qReal::Id(), this));
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
