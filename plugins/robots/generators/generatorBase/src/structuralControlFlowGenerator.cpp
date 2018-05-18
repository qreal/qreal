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
SemanticNode *StructuralControlFlowGenerator::transformNode(myUtils::IntermediateNode *node)
{
	switch (node->type()) {
	case myUtils::IntermediateNode::Type::simple: {
		myUtils::SimpleNode *simpleNode = dynamic_cast<myUtils::SimpleNode *>(node);
		qReal::Id id = simpleNode->id();
		return mSemanticTree->produceNodeFor(id);
	}
	case myUtils::IntermediateNode::Type::block: {
		myUtils::BlockNode *blockNode = dynamic_cast<myUtils::BlockNode *>(node);
		semantics::ZoneNode *zone = new semantics::ZoneNode(mSemanticTree);
		zone->appendChildren({transformNode(blockNode->firstNode()), transformNode(blockNode->secondNode())});
		return zone;
	}
	case myUtils::IntermediateNode::Type::ifThenElseCondition: {
		myUtils::BlockNode *blockNode = dynamic_cast<myUtils::BlockNode *>(node);
		semantics::ZoneNode *zone = new semantics::ZoneNode(mSemanticTree);
		zone->appendChildren({transformNode(blockNode->firstNode()), transformNode(blockNode->secondNode())});
		return zone;
	}

	}
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
