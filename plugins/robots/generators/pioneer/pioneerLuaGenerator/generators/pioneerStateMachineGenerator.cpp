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

void PioneerStateMachineGenerator::performGeneration()
{
	mSemanticTreeManager.reset(new SemanticTreeManager(*mSemanticTree, mErrorReporter, mErrorsOccured));
	GotoControlFlowGenerator::performGeneration();
}

void PioneerStateMachineGenerator::visitRegular(const qReal::Id &id, const QList<LinkInfo> &links)
{
	// Base class method checks for subprogram calls, which is irrelevant for now, but does not hurt and hopefully
	// will be needed later.
	ControlFlowGeneratorBase::visitRegular(id, links);

	qDebug() << "Generating" << id.toString();

	const QList<NonZoneNode *> nodesWithThisId = mSemanticTreeManager->nodes(id);
	for (auto thisNode : nodesWithThisId) {
		SemanticNode *nextNode = nullptr;
		const qReal::Id target = links[0].target;

		if (mAsynchronousNodes.contains(id.element())) {
			if (mSemanticTree->findNodeFor(target)) {
				// thisNode is asyncronous node that transfers control to already visited node.
				// Generated code for thisNode will initiate asynchronous action and all we need to do is to generate
				// transition to a state which will execute target block when this block finishes its asynchronous
				// operation.
				nextNode = produceGotoNode(target);
				mSemanticTreeManager->addAfter(thisNode, nextNode);

				if (!mLabeledNodes.contains(target)) {
					//
					// Target node, despite being already visited, does not have a label, it means that it is a part of
					// a synchronous fragment. We copy that fragment from this node to the first asyncronous node and
					// label a start of the copied fragment. At the end of the fragment we will generate said
					// asynchronous node, which will initiate asynchronous operation, and then transition to its next
					// state, which will continue execution when operation is done.
					//
					// But first we need to lift copied fragment to a top level, since nextNode may be inside If branch.
					// Labeling something inside If branch will make generated code uncompilable.
					// Getting parent node (i.e. If statement to the branch of which our node belongs).
					//
					while (!mSemanticTreeManager->isTopLevelNode(nextNode)) {
						nextNode = mSemanticTreeManager->parent(nextNode);
					}

					auto sibling = mSemanticTreeManager->rightSibling(nextNode);
					while (sibling != nullptr && sibling->id().element() == "EndOfHandler") {
						nextNode = mSemanticTreeManager->rightSibling(nextNode);
						sibling = mSemanticTreeManager->rightSibling(nextNode);
					}

					nextNode = copySynchronousFragment(nextNode, target, true);
				}

				if (mSemanticTreeManager->isTopLevelNode(thisNode)) {
					SemanticNode * const endNode = produceEndOfHandlerNode();
					mSemanticTreeManager->addAfter(nextNode, endNode);
				}
			} else {
				// thisNode is asynchronous node that transfers control to a node that has not been visited yet. Generating
				// transition into a state associated with that node and then a new handler for target node itself.
				nextNode = mSemanticTreeManager->produceLabeledNode(target);
				if (!nextNode) {
					return;
				} else {
					mLabeledNodes << nextNode->id();
				}

				SemanticNode * const gotoNode = produceGotoNode(target);
				mSemanticTreeManager->addAfter(thisNode, gotoNode);

				// Labeled node can not be a part of a zone (i.e. "then" or "else" branch), it shall be generated in top
				// level zone.
				if (mSemanticTreeManager->isTopLevelNode(thisNode)) {
					SemanticNode * const endNode = produceEndOfHandlerNode();
					mSemanticTreeManager->addAfter(gotoNode, endNode);
					mSemanticTreeManager->addAfter(endNode, nextNode);
				} else {
					// Getting parent node (i.e. If statement to the branch of which our node belongs).
					NonZoneNode *aParent = mSemanticTreeManager->parent(thisNode);
					while (!mSemanticTreeManager->isTopLevelNode(aParent)) {
						aParent = mSemanticTreeManager->parent(aParent);
					}

					// Skipping "end" that finishes handler with If.
					SemanticNode * const endOfHandler = mSemanticTreeManager->findSibling(
							aParent
							, [](SemanticNode *node){ return node->id().element() == "EndOfHandler"; });

					if (!endOfHandler) {
						mErrorReporter.addError(tr("Can not find end of an If statement, generation internal error or "
								"too complex algorithmic construction."));
						mErrorsOccured = true;
						return;
					}

					// Adding our labeled node denoting new handler after the end of a handler with If node.
					mSemanticTreeManager->addAfter(endOfHandler, nextNode);
				}
			}
		} else {
			if (!mSemanticTree->findNodeFor(target)) {
				// It is not an asynchronous node, generating as-is.
				nextNode = mSemanticTree->produceNodeFor(target);
				mSemanticTreeManager->addAfter(thisNode, nextNode);
			} else {
				// Synchronous node leading to already visited node. Need some copypasting of synchronous fragments,
				// or else we will stall the program waiting for an event that was never initiated.
				copySynchronousFragment(thisNode, target, false);
			}
		}
	}
}

void PioneerStateMachineGenerator::visitConditional(const qReal::Id &id, const QList<LinkInfo> &links)
{
	Q_UNUSED(links)

	qDebug() << "Generating conditional" << id.toString();


	const QPair<LinkInfo, LinkInfo> branches(ifBranchesFor(id));
	const LinkInfo thenLink = branches.first;
	const LinkInfo elseLink = branches.second;

	auto nodes = mSemanticTreeManager->nodes(id);
	for (auto node : nodes) {
		IfNode * const thisNode = static_cast<IfNode *>(node);

		mSemanticTreeManager->addToZone(thisNode->thenZone(), thenLink.target);
		mSemanticTreeManager->addToZone(thisNode->elseZone(), elseLink.target);

		if (mSemanticTreeManager->isTopLevelNode(thisNode)) {
			SemanticNode * const endNode = produceEndOfHandlerNode();
			mSemanticTreeManager->addAfter(thisNode, endNode);
		}
	}
}

void PioneerStateMachineGenerator::visitFinal(const qReal::Id &id, const QList<LinkInfo> &links)
{
	generatorBase::GotoControlFlowGenerator::visitFinal(id, links);

	qDebug() << "Generating final " << id.toString();

	// Here we are going to add finishing end-of-handler node in case it is missing (for example, diagrams like
	// "Initial Node" -> "Final Node" will not generate it automatically).
	// It is a kind of hack because asynchronous handler shall be a first-class entity and a zone node.

	auto nodes = mSemanticTreeManager->nodes(id);
	for (auto node : nodes) {
		SimpleNode * const thisNode = static_cast<SimpleNode *>(node);

		// Getting root node.
		NonZoneNode *aParent = mSemanticTreeManager->parent(thisNode);
		while (!mSemanticTreeManager->isTopLevelNode(aParent)) {
			aParent = mSemanticTreeManager->parent(aParent);
		}

		// Searching for end-of-handler node.
		SemanticNode * endOfHandler = mSemanticTreeManager->findSibling(
				aParent
				, [](SemanticNode *node){ return node->id().element() == "EndOfHandler"; });

		if (!endOfHandler) {
			// If not found, create and add one.
			endOfHandler = produceEndOfHandlerNode();
			mSemanticTreeManager->addAfter(thisNode, endOfHandler);
		}
	}
}

void PioneerStateMachineGenerator::visit(const qReal::Id &nodeId, QList<utils::DeepFirstSearcher::LinkInfo> &links)
{
	generatorBase::GotoControlFlowGenerator::visit(nodeId, links);
	for (const auto &hook : mNodeHooks) {
		hook(nodeId);
	}
}

SemanticNode *PioneerStateMachineGenerator::copySynchronousFragment(
		SemanticNode *after
		, const qReal::Id &from
		, bool withLabel)
{
	qDebug() << "Copying synchronous fragment from "
			<< from << "after" << after->id() << ", with label = " << withLabel;

	// Here "from" may have many corresponding nodes in a semantic tree, but any node will do, so using findNodeFor.
	NonZoneNode *oldTarget = dynamic_cast<NonZoneNode *>(mSemanticTree->findNodeFor(from));
	if (!oldTarget) {
		/// @todo: actually, why not?
		mErrorReporter.addError(tr("Can not close a loop on algorithmic block."));
		mErrorsOccured = true;
		return nullptr;
	}

	NonZoneNode *fragmentStartNode = withLabel
			? mSemanticTreeManager->produceLabeledNode(from)
			: dynamic_cast<NonZoneNode *>(mSemanticTree->produceNodeFor(from));

	if (!fragmentStartNode) {
		return nullptr;
	} else {
		mLabeledNodes << fragmentStartNode->id();
	}

	if (!dynamic_cast<NonZoneNode *>(after)) {
		mErrorReporter.addError(tr("Generation internal error, non-zone node is a start of a fragment."));
		mErrorsOccured = true;
		return nullptr;
	}

	if (withLabel && !dynamic_cast<IfNode *>(after) && after->id().element() != "EndOfHandler") {
		// End-of-handler shall go before every labeled node, since label here is actually a start of a new handler.
		// "If" nodes have their own end-of-handler.
		SemanticNode * const endNode = produceEndOfHandlerNode();
		mSemanticTreeManager->addAfter(after, endNode);
		mSemanticTreeManager->addAfter(endNode, fragmentStartNode);
	} else {
		mSemanticTreeManager->addAfter(after, fragmentStartNode);
	}

	if (isAsynchronous(fragmentStartNode)) {
		// Synchronous fragment is trivial and its first node is asynchronous. Generating transition from it and we're
		// done here.
		//
		// Using oldTarget because fragmentStartNode was just added and does not have siblings, but it is a copy
		// of oldTarget.
		auto rightSibling = mSemanticTreeManager->rightSibling(oldTarget);
		if (rightSibling) {
			while (rightSibling != nullptr && rightSibling->id().element() == "EndOfHandler") {
				rightSibling = mSemanticTreeManager->rightSibling(rightSibling);
			}

			if (rightSibling == nullptr) {
				return nullptr;
			}

			auto gotoNode = produceGotoNode(rightSibling->id());
			fragmentStartNode->appendSibling(gotoNode);
			return gotoNode;
		} else {
			mErrorReporter.addError(tr("Generation internal error, asynchronous fragment start node generation " \
					"failed."));
			mErrorsOccured = true;
		}

		return nullptr;
	}

	auto siblings = mSemanticTreeManager->copyRightSiblingsUntil(
			oldTarget
			, [this](SemanticNode * node){ return isAsynchronous(node); });

	if (siblings.isEmpty()) {
		// Fragment is trivial and non-asynchronous --- so it must be FinalNode. Fine, no need to copy it.
		if (fragmentStartNode->id().element() != "FinalNode") {
			mErrorReporter.addError(tr("Generation internal error, program ends abruptly."));
			mErrorsOccured = true;
			return nullptr;
		}

		return fragmentStartNode;
	}

	if (isAsynchronous(siblings.last())) {
		// Synchronous fragment finished with asynchronous node
		fragmentStartNode->appendSiblings(siblings);

		// Now we shall look for the end of original fragment: find asynchronous node on which a fragment shall be
		// ending and get its target node. If it is a simple loop, asynchronous node shall be already visited, and
		// shall have one target node. If it is an If statement, If already can take care of transferring control
		// through asynchronous nodes in its branches.
		auto asynchronousNode = mSemanticTreeManager->findSibling(
				oldTarget
				, [this](SemanticNode * node){ return isAsynchronous(node); });

		if (!dynamic_cast<IfNode *>(asynchronousNode)) {
			auto asynchronousNodeTarget = mSemanticTreeManager->rightSibling(asynchronousNode);
			if (!asynchronousNodeTarget) {
				mErrorReporter.addError(tr("Generation internal error, asynchronous node does not have target node."));
				mErrorsOccured = true;
				return nullptr;
			}

			while (asynchronousNodeTarget != nullptr && asynchronousNodeTarget->id().element() == "EndOfHandler") {
				asynchronousNodeTarget = mSemanticTreeManager->rightSibling(asynchronousNodeTarget);
			}

			if (asynchronousNodeTarget == nullptr) {
				return nullptr;
			}

			auto gotoNode = produceGotoNode(asynchronousNodeTarget->id());
			fragmentStartNode->appendSibling(gotoNode);
			return gotoNode;
		} else {
			auto ifNode = dynamic_cast<IfNode *>(siblings.last());
			// This If node is synthetic, so "visitConditional" will never be called for it, we need to check for
			// endNode ourselves.
			if (mSemanticTreeManager->isTopLevelNode(ifNode)) {
				SemanticNode * const endNode = produceEndOfHandlerNode();
				mSemanticTreeManager->addAfter(ifNode, endNode);
				return endNode;
			}

			return ifNode;
		}
	} else {
		mErrorReporter.addError(tr("Purely synchronous loops or If branches are not supported yet."));
		mErrorsOccured = true;
	}

	return nullptr;
}

bool PioneerStateMachineGenerator::isAsynchronous(const SemanticNode * const node) const
{
	// A node is asynchronous if it itself is asynchronous or if it is If node and its branches are asynchronous.
	if (mAsynchronousNodes.contains(node->id().element())) {
		return true;
	}

	if (dynamic_cast<IfNode const * const>(node)) {
		// Do not bothering to look inside If branches, it does not support synchronous branches anyway and will produce
		// error message by itself in such case.
		return true;
	}

	return false;
}

SemanticNode *PioneerStateMachineGenerator::produceEndOfHandlerNode()
{
	qReal::Id syntheticId = qReal::Id::createElementId("synthetic", "synthetic", "EndOfHandler");
	SimpleNode * const result = mSemanticTree->produceSimple(syntheticId);
	// No need for special handling, from the point of view of a generator it is just some simple node.
	result->bindToSyntheticConstruction(SimpleNode::noSytheticBinding);
	return result;
}
