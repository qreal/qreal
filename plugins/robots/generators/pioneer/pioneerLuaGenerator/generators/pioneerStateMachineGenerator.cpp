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

#include "pioneerStateMachineGenerator.h"

#include <generatorBase/semanticTree/semanticNode.h>
#include <generatorBase/semanticTree/simpleNode.h>

using namespace pioneer::lua;
using namespace generatorBase;
using namespace generatorBase::semantics;

PioneerStateMachineGenerator::PioneerStateMachineGenerator(
		const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, generatorBase::GeneratorCustomizer &customizer
		, generatorBase::PrimaryControlFlowValidator &validator
		, const qReal::Id &diagramId
		, QObject *parent
		, bool isThisDiagramMain)
	: GotoControlFlowGenerator(repo, errorReporter, customizer, validator, diagramId, parent, isThisDiagramMain)
{
	mAsynchronousNodes << "GeoTakeoff" << "GeoLanding" << "GoToPoint";
}

void PioneerStateMachineGenerator::registerNodeHook(std::function<void(const qReal::Id)> hook)
{
	mNodeHooks.append(hook);
}

void PioneerStateMachineGenerator::visitRegular(const qReal::Id &id, const QList<LinkInfo> &links)
{
	// Base class method checks for subprogram calls, which is irrelevant for now, but does not hurt and hopefully
	// will be needed later.
	ControlFlowGeneratorBase::visitRegular(id, links);

	SimpleNode * const thisNode = static_cast<SimpleNode *>(mSemanticTree->findNodeFor(id));
	SemanticNode *nextNode = nullptr;
	const qReal::Id target = links[0].target;

	if (mAsynchronousNodes.contains(id.element())) {
		if (mSemanticTree->findNodeFor(target)) {
			// thisNode is asyncronous node that transfers control to already visited node.
			// Generated code for thisNode will initiate asynchronous action and all we need to do is to generate
			// transition to a state which will execute target block when this block finishes its asynchronous
			// operation.
			nextNode = produceGotoNode(target);
			thisNode->insertSiblingAfterThis(nextNode);
			SemanticNode * const endNode = produceEndOfHandlerNode();
			static_cast<NonZoneNode*>(nextNode)->insertSiblingAfterThis(endNode);

			if (!mLabeledNodes.contains(target)) {
				//
				// Target node, despite being already visited, does not have a label, it means that it is a part of a
				// synchronous fragment. We copy that fragment from this node to the first asyncronous node and
				// label a start of the copied fragment. At the end of the fragment we will generate said asynchronous
				// node, which will initiate asynchronous operation, and then transition to its next state, which will
				// continue execution when operation is done.
				//
				// It will confuse "findNodeFor(id)" (there will be many semantic nodes for a block with given id), but
				// we actually do not care which copy will be used later, since they are the same.
				//
				copySynchronousFragment(nextNode, target, true);
			}
		} else {
			// thisNode is asynchronous node that transfers control to a node that has not been visited yet. Generating
			// transition into a state associated with that node and then a new handler for target node itself.
			nextNode = produceLabeledNode(target);
			if (!nextNode) {
				return;
			}

			SemanticNode *gotoNode = produceGotoNode(target);
			thisNode->insertSiblingAfterThis(gotoNode);
			SemanticNode * const endNode = produceEndOfHandlerNode();
			dynamic_cast<NonZoneNode *>(gotoNode)->insertSiblingAfterThis(endNode);
			dynamic_cast<NonZoneNode *>(endNode)->insertSiblingAfterThis(nextNode);
		}
	} else {
		if (!mSemanticTree->findNodeFor(target)) {
			// It is not an asynchronous node, generating as-is.
			nextNode = mSemanticTree->produceNodeFor(target);
			thisNode->insertSiblingAfterThis(nextNode);
		} else {
			// Synchronous node leading to already visited node. Need some copypasting of synchronous fragments,
			// or else we will stall the program waiting for an event that was never initiated.
			copySynchronousFragment(thisNode, target, false);
		}
	}
}

void PioneerStateMachineGenerator::visitConditional(const qReal::Id &id, const QList<LinkInfo> &links)
{
	Q_UNUSED(links)

	const QPair<LinkInfo, LinkInfo> branches(ifBranchesFor(id));
	const LinkInfo thenLink = branches.first;
	const LinkInfo elseLink = branches.second;

	IfNode * const thisNode = static_cast<IfNode *>(mSemanticTree->findNodeFor(id));
	thisNode->thenZone()->appendChild(produceGotoNode(thenLink.target));
	thisNode->elseZone()->appendChild(produceGotoNode(elseLink.target));

	produceNextNodeIfNeeded(thenLink, thisNode);
	produceNextNodeIfNeeded(elseLink, thisNode);

	SemanticNode * const endNode = produceEndOfHandlerNode();
	dynamic_cast<NonZoneNode *>(thisNode)->insertSiblingAfterThis(endNode);
}

void PioneerStateMachineGenerator::visit(const qReal::Id &nodeId, QList<utils::DeepFirstSearcher::LinkInfo> &links)
{
	generatorBase::GotoControlFlowGenerator::visit(nodeId, links);
	for (const auto &hook : mNodeHooks) {
		hook(nodeId);
	}
}

void PioneerStateMachineGenerator::copySynchronousFragment(SemanticNode *after, const qReal::Id from, bool withLabel)
{
	NonZoneNode *oldTarget = dynamic_cast<NonZoneNode *>(mSemanticTree->findNodeFor(from));
	if (!oldTarget) {
		/// @todo: actually, why not?
		mErrorReporter.addError(tr("Can not close a loop on algorithmic block."));
		mErrorsOccured = true;
		return;
	}

	NonZoneNode *fragmentStartNode = withLabel
			? produceLabeledNode(from)
			: dynamic_cast<NonZoneNode *>(mSemanticTree->produceNodeFor(from));

	if (!fragmentStartNode) {
		return;
	}

	if (!dynamic_cast<NonZoneNode *>(after)) {
		/// @todo: actually, why not?
		mErrorReporter.addError(tr("Can not close a loop right after algorithmic block."));
		mErrorsOccured = true;
		return;
	}

	dynamic_cast<NonZoneNode *>(after)->insertSiblingAfterThis(fragmentStartNode);

	if (isAsynchronous(fragmentStartNode)) {
		// Synchronous fragment is trivial and its first node is asynchronous. Generating transition from it and we're
		// done here.
		//
		// Using oldTarget because fragmentStartNode was just added and does not have siblings, but it is a copy
		// of oldTarget.
		const auto rightSibling = findRightSibling(oldTarget);
		if (rightSibling) {
			auto gotoNode = produceGotoNode(rightSibling->id());
			fragmentStartNode->appendSibling(gotoNode);
		} else {
			mErrorReporter.addError(tr("Generation internal error, asynchronous fragment start node generation " \
					"failed."));
			mErrorsOccured = true;
		}

		return;
	}

	auto siblings = copyRightSiblingsUntilAsynchronous(oldTarget);
	if (siblings.isEmpty()) {
		mErrorReporter.addError(tr("Loop can not be closed on a block that is last in its structural construct."));
		mErrorsOccured = true;
		return;
	}

	if (isAsynchronous(siblings.last())) {
		// Synchronous fragment finished with asynchronous node
		fragmentStartNode->appendSiblings(siblings);

		// Now we shall look for the end of original fragment: find asynchronous node on which a fragment shall be
		// ending and get its target node. Assuming that they are already visited (here we assuming that it is a loop,
		// may not be the case when logical branching blocks will be introduced).
		auto asynchronousNode = findAsynchronousSibling(dynamic_cast<NonZoneNode *>(oldTarget));
		const auto asynchronousNodeTarget = findRightSibling(dynamic_cast<NonZoneNode *>(asynchronousNode));
		if (!asynchronousNodeTarget) {
			mErrorReporter.addError(tr("Generation internal error, asynchronous node does not have target node."));
			mErrorsOccured = true;
			return;
		}

		auto gotoNode = produceGotoNode(asynchronousNodeTarget->id());
		fragmentStartNode->appendSibling(gotoNode);
	} else {
		mErrorReporter.addError(tr("Purely synchronous loops are not supported yet."));
		mErrorsOccured = true;
	}
}

const QLinkedList<SemanticNode *> PioneerStateMachineGenerator::copyRightSiblingsUntilAsynchronous(NonZoneNode *node)
{
	const auto zone = node->parentZone();
	if (!zone) {
		mErrorReporter.addError(tr("Generation internal error, synchronous fragment zone is absent."));
		mErrorsOccured = true;
		return {};
	}

	SemanticNode * currentChild = node;
	QLinkedList<SemanticNode *> result;
	while (zone->nextChild(currentChild)) {
		currentChild = zone->nextChild(currentChild);

		if (isSynthetic(currentChild)) {
			// Synthetic node, we want to simply skip it, as it is most probably transition node for asynchronous block,
			// it will be generated anyway.
			continue;
		}

		result << mSemanticTree->produceNodeFor(currentChild->id());
		if (isAsynchronous(currentChild)) {
			break;
		}
	}

	return result;
}

NonZoneNode *PioneerStateMachineGenerator::produceLabeledNode(const qReal::Id block)
{
	NonZoneNode *node = dynamic_cast<NonZoneNode *>(mSemanticTree->produceNodeFor(block));
	if (!node) {
		/// @todo: Well, actually why not... But for now it is ok, we don't have zone nodes on a diagram.
		mErrorReporter.addError(QString(tr("Generation internal error, please send bug report to developers."
				"Additional info: zone node %1 can not be used as labeled node.").arg(block.id())));
		mErrorsOccured = true;
		return nullptr;
	}

	node->addLabel();
	mLabeledNodes << block;
	return node;
}

bool PioneerStateMachineGenerator::isAsynchronous(const SemanticNode * const node) const
{
	return mAsynchronousNodes.contains(node->id().element());
}

bool PioneerStateMachineGenerator::isSynthetic(const SemanticNode * const node) const
{
	return node->id().editor().startsWith("label_");
}

SemanticNode *PioneerStateMachineGenerator::findRightSibling(NonZoneNode * const node) const
{
	if (!node) {
		return nullptr;
	}

	const auto zone = node->parentZone();
	 SemanticNode * currentChild = node;
	if (zone && zone->nextChild(currentChild)) {
		currentChild = zone->nextChild(currentChild);
		while (isSynthetic(currentChild)) {
			currentChild = zone->nextChild(currentChild);
			continue;
		}

		return currentChild;
	} else {
		return nullptr;
	}
}

SemanticNode * PioneerStateMachineGenerator::findAsynchronousSibling(NonZoneNode *node) const
{
	const auto zone = node->parentZone();
	if (!zone) {
		return nullptr;
	}

	SemanticNode * currentChild = node;
	while (zone->nextChild(currentChild)) {
		currentChild = zone->nextChild(currentChild);
		if (isAsynchronous(currentChild)) {
			return currentChild;
		}
	}

	return nullptr;
}

SemanticNode *PioneerStateMachineGenerator::produceEndOfHandlerNode()
{
	qReal::Id syntheticId = qReal::Id::createElementId("synthetic", "synthetic", "EndOfHandler");
	SimpleNode * const result = mSemanticTree->produceSimple(syntheticId);
	// No need for special handling, from the point of view of a generator it is just some simple node.
	result->bindToSyntheticConstruction(SimpleNode::noSytheticBinding);
	return result;
}
