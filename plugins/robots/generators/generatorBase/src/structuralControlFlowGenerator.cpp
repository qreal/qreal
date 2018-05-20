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
	, mStructurizator(nullptr)
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

	mStructurizator = new Structurizator(mRepo, mIds, this);
	myUtils::IntermediateNode *tree = mStructurizator->performStructurization();
	obtainSemanticTree(tree);
}

void StructuralControlFlowGenerator::obtainSemanticTree(myUtils::IntermediateNode *root)
{
	Q_UNUSED(root)
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

	default:
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
	ZoneNode *zone = new ZoneNode(mSemanticTree);

	if (blockNode->firstNode()->type() == myUtils::IntermediateNode::simple) {
		const myUtils::SimpleNode *simpleNode = dynamic_cast<const myUtils::SimpleNode *>(blockNode->firstNode());

		switch (semanticsOf(simpleNode->id())) {
		case enums::semantics::conditionalBlock:
		case enums::semantics::switchBlock:
			break;
		default:
			zone->appendChild(transformSimple(simpleNode));
		}

	} else {
		zone->appendChild(transformBlock(blockNode->firstNode()));
	}

	zone->appendChild(transformNode(blockNode->secondNode()));
	return zone;
}

SemanticNode *StructuralControlFlowGenerator::transformIfThenElse(const myUtils::IntermediateNode *node)
{
	const myUtils::IfNode *ifNode = dynamic_cast<const myUtils::IfNode *>(node);
	const qReal::Id conditionId = ifNode->condition()->id();
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
				semanticSwitch->generateIfs();
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
			&& semanticsOf(whileNode->headNode()->firstId()) == enums::semantics::conditionalBlock) {
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

	if (semanticsOf(conditionId) == enums::semantics::switchBlock) {
		SwitchNode *semanticSwitch = new SwitchNode(conditionId, mSemanticTree);

		for (const qReal::Id &link : mRepo.outgoingLinks(conditionId)) {
			const QString expression = mRepo.property(link, "Guard").toString();
			const qReal::Id otherVertex = mRepo.otherEntityFromLink(link, conditionId);

			for (const myUtils::IntermediateNode *branchNode : branches) {
				if (node->firstId() == otherVertex) {
					semanticSwitch->addBranch(expression, transformNode(branchNode));
					break;
				}
			}
		}

		if (switchNode->hasBreakInside()) {
			semanticSwitch->generateIfs();
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

SemanticNode *StructuralControlFlowGenerator::transformIfWithBreak(const myUtils::IntermediateNode *node)
{
	const myUtils::IfWithBreakNode *ifWithBreakNode = dynamic_cast<const myUtils::IfWithBreakNode *>(node);
	const qReal::Id conditionId = ifWithBreakNode->condition()->firstId();

	if (semanticsOf(conditionId) == enums::semantics::conditionalBlock) {
		IfNode *semanticIf= new IfNode(conditionId, mSemanticTree);

		QPair<LinkInfo, LinkInfo> branches = ifBranchesFor(conditionId);

		if (ifWithBreakNode->actionsBeforeBreak()) {
			semanticIf->thenZone()->appendChild(transformNode(ifWithBreakNode->actionsBeforeBreak()));
		}

		// check inverting condition
		if (branches.first.target != ifWithBreakNode->nodeThatIsConnectedWithCondition()->firstId()) {
			semanticIf->invertCondition();
		}

		semanticIf->thenZone()->appendChild(semantics::SimpleNode::createBreakNode(mSemanticTree));

		return semanticIf;

	} else if (semanticsOf(conditionId) == enums::semantics::switchBlock) {
		SwitchNode *semanticSwitch = new SwitchNode(conditionId, mSemanticTree);

		// deal with semanticSwitch
		return semanticSwitch;
	}


	qDebug() << "Problem: couldn't identify semantics id for If with break";
	mCantBeGeneratedIntoStructuredCode = true;

	return nullptr;
}


/*
IfNode *StructuralControlFlowGenerator::createIfFromSwitch(int v, int bodyNumber)
{
	qReal::Id vId = mMapVertexLabel.key(v);
	bool needInverting = true;
	QList<qReal::Id> links;
	bool hasDefaultBranch = false;
	for (const int edge : mFollowers2[v][bodyNumber]) {
		links.append(mEdges[edge]);
		if (mRepo.property(mEdges[edge], "Guard").toString().isEmpty()) {
			hasDefaultBranch = true;
		}
	}

	if (hasDefaultBranch) {
		needInverting = false;
		links.clear();
		for (const int u : mFollowers2[v].keys()) {
			if (u == bodyNumber) {
				continue;
			}

			for (const int edge : mFollowers2[v][u]) {
				links.append(mEdges[edge]);
			}
		}
	}

	IfNode *ifNode = IfNode::fromSwitchCase(vId, links);

	if (needInverting) {
		ifNode->invertCondition();
	}

	ifNode->setCondition(constructConditionFromSwitch(vId, links));

	return ifNode;
}

QString StructuralControlFlowGenerator::constructConditionFromSwitch(const Id &id, const QList<Id> &links) const
{
	const QString expression = mRepo.property(id, "Expression").toString();
	QString condition = "";
	for (const qReal::Id &link : links) {
		condition += "(" + expression + " == " + mRepo.property(link, "Guard").toString() + ") || ";
	}
	condition.chop(4);
	return condition;
}
*/
