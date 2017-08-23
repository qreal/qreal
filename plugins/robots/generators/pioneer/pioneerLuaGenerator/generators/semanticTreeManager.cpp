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

#include "semanticTreeManager.h"

#include <generatorBase/semanticTree/semanticTree.h>

using namespace pioneer::lua;
using namespace generatorBase::semantics;

SemanticTreeManager::SemanticTreeManager(
		SemanticTree &semanticTree
		, qReal::ErrorReporterInterface &errorReporter
		, bool &errorsOccured)
	: mSemanticTree(semanticTree)
	, mErrorReporter(errorReporter)
	, mErrorsOccured(errorsOccured)
{
}

bool SemanticTreeManager::isGotoNode(const SemanticNode * const node)
{
	return node->id().editor().startsWith("label_");
}

bool SemanticTreeManager::isSynthetic(const SemanticNode * const node)
{
	return isGotoNode(node) || node->id().editor() == "synthetic";
}

SemanticNode *SemanticTreeManager::nonSyntheticRightSibling(SemanticNode * const node)
{
	NonZoneNode * const nonZoneNode = dynamic_cast<NonZoneNode * const>(node);
	if (!nonZoneNode) {
		return nullptr;
	}

	const auto zone = nonZoneNode->parentZone();
	SemanticNode * currentChild = nonZoneNode;
	if (zone && zone->nextChild(currentChild)) {
		currentChild = zone->nextChild(currentChild);
		while (currentChild && isSynthetic(currentChild)) {
			currentChild = zone->nextChild(currentChild);
			continue;
		}

		return currentChild;
	} else {
		return nullptr;
	}
}

SemanticNode *SemanticTreeManager::anyRightSibling(SemanticNode * const node)
{
	NonZoneNode * const nonZoneNode = dynamic_cast<NonZoneNode * const>(node);
	if (!nonZoneNode) {
		return nullptr;
	}

	const auto zone = nonZoneNode->parentZone();
	if (zone) {
		return zone->nextChild(nonZoneNode);
	} else {
		return nullptr;
	}
}

NonZoneNode *SemanticTreeManager::parent(SemanticNode * const node)
{
	NonZoneNode * const nonZoneNode = dynamic_cast<NonZoneNode * const>(node);
	if (!nonZoneNode) {
		return nullptr;
	}

	return static_cast<NonZoneNode *>(nonZoneNode->parentZone()->parentNode());
}

NonZoneNode *SemanticTreeManager::topLevelParent(SemanticNode * const node)
{
	NonZoneNode *aParent = parent(node);
	while (!isTopLevelNode(aParent)) {
		aParent = parent(aParent);
	}

	return aParent;
}

void SemanticTreeManager::addAfter(SemanticNode * const thisNode, SemanticNode * const nextNode)
{
	static_cast<NonZoneNode * const>(thisNode)->insertSiblingAfterThis(nextNode);
}

bool SemanticTreeManager::isTopLevelNode(const generatorBase::semantics::SemanticNode * const node)
{
	if (!static_cast<const NonZoneNode * const>(node)->parentZone()) {
		return true;
	}

	const SemanticNode * const parent = static_cast<const NonZoneNode * const>(node)->parentZone()->parentNode();
	return dynamic_cast<const RootNode * const>(parent) != nullptr;
}

void SemanticTreeManager::addToZone(generatorBase::semantics::ZoneNode * const zone, const qReal::Id &id)
{
	zone->appendChild(produceNode(id));
}

NonZoneNode *SemanticTreeManager::produceLabeledNode(const qReal::Id &block)
{
	NonZoneNode *node = produceNode(block);

	if (!node) {
		reportError(QObject::tr("Generation internal error, please send bug report to developers."
				"Additional info: zone node %1 can not be used as labeled node.").arg(block.id()));
		return nullptr;
	}

	node->addLabel();
	return node;
}

SemanticNode *SemanticTreeManager::findSibling(SemanticNode *node, std::function<bool(SemanticNode *)> predicate)
{
	NonZoneNode *nonZoneNode = dynamic_cast<NonZoneNode *>(node);
	if (!nonZoneNode) {
		return nullptr;
	}

	const auto zone = nonZoneNode->parentZone();
	if (!zone) {
		return nullptr;
	}

	SemanticNode * currentChild = nonZoneNode;
	while (zone->nextChild(currentChild)) {
		currentChild = zone->nextChild(currentChild);
		if (predicate(currentChild)) {
			return currentChild;
		}
	}

	return nullptr;
}

QLinkedList<SemanticNode *> SemanticTreeManager::copyRightSiblingsUntil(SemanticNode *node
		, std::function<bool(SemanticNode *)> predicate)
{
	NonZoneNode *nonZoneNode = dynamic_cast<NonZoneNode *>(node);
	if (!nonZoneNode) {
		reportError(QObject::tr("Generation internal error, synchronous zone parent is a zone node."));
		return {};
	}

	const auto zone = nonZoneNode->parentZone();
	if (!zone) {
		reportError(QObject::tr("Generation internal error, synchronous fragment zone is absent."));
		return {};
	}

	NonZoneNode * currentChild = nonZoneNode;
	QLinkedList<SemanticNode *> result;
	while (zone->nextChild(currentChild)) {
		currentChild = dynamic_cast<NonZoneNode *>(zone->nextChild(currentChild));

		if (!currentChild) {
			reportError(QObject::tr("Generation internal error, zone contains zone node."));
			return {};
		}

		if (isGotoNode(currentChild)) {
			// Goto node, we want to simply skip it, as it is not actually a part of a fragment.
			continue;
		}

		result << copy(currentChild);

		if (predicate(currentChild)) {
			break;
		}
	}

	return result;
}

QList<NonZoneNode *> SemanticTreeManager::nodes(const qReal::Id &id) const
{
	if (mClones.contains(id)) {
		QList<NonZoneNode *> result;
		for (const auto &clone : mClones.values(id)) {
			result << clone.clone;
		}

		return result;
	} else {
		return { mSemanticTree.findNodeFor(id) };
	}
}

QList<NonZoneNode *> SemanticTreeManager::clones(NonZoneNode *node) const
{
	if (!node) {
		return {};
	}

	if (!mClones.contains(node->id())) {
		return {};
	}

	QList<NonZoneNode *> result;
	for (const CloneInfo &cloneInfo : mClones.values(node->id())) {
		if (isParent(node, cloneInfo)) {
			result << cloneInfo.clone;
		}
	}

	return result;
}

bool SemanticTreeManager::isParent(NonZoneNode *node, const SemanticTreeManager::CloneInfo &cloneInfo) const
{
	if (node == cloneInfo.parent) {
		return true;
	}

	for (const CloneInfo &parent : mClones.values(node->id())) {
		if (parent.clone == cloneInfo.parent) {
			if (isParent(node, parent)) {
				return true;
			}
		}
	}

	return false;
}

NonZoneNode *SemanticTreeManager::produceNode(const qReal::Id &id)
{
	auto node = mSemanticTree.findNodeFor(id);
	if (node) {
		return copy(node);
	}

	return static_cast<NonZoneNode *>(mSemanticTree.produceNodeFor(id));
}

NonZoneNode *SemanticTreeManager::copy(NonZoneNode *node)
{
	qDebug() << "Cloning" << node->id();

	if (isGotoNode(node)) {
		SimpleNode * const gotoNode = mSemanticTree.produceSimple(node->id());
		gotoNode->bindToSyntheticConstruction(SimpleNode::gotoNode);
		return gotoNode;
	}

	NonZoneNode * const result = static_cast<NonZoneNode *>(mSemanticTree.produceNodeFor(node->id()));
	registerClone(node, result);

	// "If" needs special handling, it has complex inner structure that shall be copied.
	auto ifNode = dynamic_cast<IfNode *>(node);
	if (ifNode) {
		auto newIfNode = dynamic_cast<IfNode *>(result);
		copyIfBranch(ifNode->thenZone(), newIfNode->thenZone());
		copyIfBranch(ifNode->elseZone(), newIfNode->elseZone());
	}

	return result;
}

void SemanticTreeManager::copyIfBranch(ZoneNode * const from, ZoneNode * const to)
{
	QLinkedList<SemanticNode *> newNodes;
	for (auto node : from->children()) {
		auto nonZoneNode = dynamic_cast<NonZoneNode *>(node);
		if (!nonZoneNode) {
			reportError(QObject::tr("Generation internal error, zone contains zone node."));
			return;
		}

		newNodes << copy(nonZoneNode);
	}

	to->appendChildren(newNodes);
}

void SemanticTreeManager::registerClone(SemanticNode * const original, SemanticNode * const clone)
{
	auto contains = [this](NonZoneNode * const node) {
		for (auto clone : mClones.values(node->id())) {
			if (clone.clone == node) {
				return true;
			}
		}

		return false;
	};

	if (!contains(static_cast<NonZoneNode *>(original))) {
		mClones.insert(original->id(), CloneInfo{static_cast<NonZoneNode *>(original), nullptr});
	}

	mClones.insert(clone->id(), CloneInfo{static_cast<NonZoneNode *>(clone), static_cast<NonZoneNode *>(original)});
}

void SemanticTreeManager::reportError(const QString &message)
{
	mErrorReporter.addError(message);
	mErrorsOccured = true;
}
