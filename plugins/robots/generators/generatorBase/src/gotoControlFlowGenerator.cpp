/* Copyright 2007-2015 QReal Research Group
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

#include "generatorBase/gotoControlFlowGenerator.h"

using namespace generatorBase;
using namespace qReal;
using namespace semantics;

GotoControlFlowGenerator::GotoControlFlowGenerator(
		const qrRepo::RepoApi &repo
		, ErrorReporterInterface &errorReporter
		, GeneratorCustomizer &customizer
		, PrimaryControlFlowValidator &validator
		, const Id &diagramId
		, QObject *parent
		, bool isThisDiagramMain)
	: ControlFlowGeneratorBase(repo, errorReporter, customizer, validator, diagramId, parent, isThisDiagramMain)
{
}

ControlFlowGeneratorBase *GotoControlFlowGenerator::cloneFor(const qReal::Id &diagramId, bool cloneForNewDiagram)
{
	GotoControlFlowGenerator * const copy = new GotoControlFlowGenerator(mRepo
			, mErrorReporter, mCustomizer, (cloneForNewDiagram ? *mValidator.clone() : mValidator)
			, diagramId, parent(), false);

	return copy;
}

void GotoControlFlowGenerator::beforeSearch()
{
}

void GotoControlFlowGenerator::visitRegular(const Id &id, const QList<LinkInfo> &links)
{
	ControlFlowGeneratorBase::visitRegular(id, links);
	SimpleNode * const thisNode = static_cast<SimpleNode *>(mSemanticTree->findNodeFor(id));
	SemanticNode *nextNode = nullptr;
	if (mSemanticTree->findNodeFor(links[0].target)) {
		nextNode = produceGotoNode(links[0].target);
	} else {
		nextNode = mSemanticTree->produceNodeFor(links[0].target);
		nextNode->addLabel();
	}

	thisNode->insertSiblingAfterThis(nextNode);
}

void GotoControlFlowGenerator::visitConditional(const Id &id, const QList<LinkInfo> &links)
{
	Q_UNUSED(links)

	QPair<LinkInfo, LinkInfo> const branches(ifBranchesFor(id));
	const LinkInfo thenLink = branches.first;
	const LinkInfo elseLink = branches.second;

	IfNode * const thisNode = static_cast<IfNode *>(mSemanticTree->findNodeFor(id));
	thisNode->thenZone()->appendChild(produceGotoNode(thenLink.target));
	thisNode->elseZone()->appendChild(produceGotoNode(elseLink.target));

	produceNextNodeIfNeeded(thenLink, thisNode);
	produceNextNodeIfNeeded(elseLink, thisNode);
}

void GotoControlFlowGenerator::visitLoop(const Id &id, const QList<LinkInfo> &links)
{
	Q_UNUSED(links)

	QPair<LinkInfo, LinkInfo> const branches(loopBranchesFor(id));
	const LinkInfo bodyLink = branches.first;
	const LinkInfo nextLink = branches.second;

	LoopNode * const thisNode = static_cast<LoopNode *>(mSemanticTree->findNodeFor(id));
	thisNode->bodyZone()->appendChild(produceGotoNode(bodyLink.target));
	thisNode->insertSiblingAfterThis(produceGotoNode(nextLink.target));

	produceNextNodeIfNeeded(bodyLink, thisNode);
	produceNextNodeIfNeeded(nextLink, thisNode);
}

void GotoControlFlowGenerator::visitSwitch(const Id &id, const QList<LinkInfo> &links)
{
	SwitchNode * const thisNode = static_cast<SwitchNode *>(mSemanticTree->findNodeFor(id));
	for (const LinkInfo &branch : links) {
		const QString value = mRepo.property(branch.linkId, "Guard").toString();
		thisNode->addBranch(value, produceGotoNode(branch.target));
		produceNextNodeIfNeeded(branch, thisNode);
	}
}

void GotoControlFlowGenerator::afterSearch()
{
}

void GotoControlFlowGenerator::performGeneration()
{
	const Id initialBlock = mSemanticTree->initialBlock();
	if (initialBlock.element() != "InitialNode") {
		// Labels for the first nodes are ignored correctly unless we are dealing with threads.
		mSemanticTree->findNodeFor(initialBlock)->addLabel();
	}

	ControlFlowGeneratorBase::performGeneration();
}

SemanticNode *GotoControlFlowGenerator::produceGotoNode(const qReal::Id &id)
{
	// Using other id to eliminate collisions with original node
	const Id gotoId("label_" + id.editor(), id.diagram(), id.element(), id.id());
	SimpleNode * const result = mSemanticTree->produceSimple(gotoId);
	result->bindToSyntheticConstruction(SimpleNode::gotoNode);
	return result;
}

void GotoControlFlowGenerator::produceNextNodeIfNeeded(const LinkInfo &info, NonZoneNode * const parent)
{
	if (!mSemanticTree->findNodeFor(info.target)) {
		SemanticNode * const nextNode = mSemanticTree->produceNodeFor(info.target);
		nextNode->addLabel();
		parent->appendSibling(nextNode);
	}
}
