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

#pragma once

#include <functional>
#include <QtCore/QLinkedList>
#include <QtCore/QMultiHash>

namespace generatorBase {
namespace semantics {
class SemanticTree;
class SemanticNode;
class NonZoneNode;
class ZoneNode;
}
}

namespace qReal {
class ErrorReporterInterface;
class Id;
}

namespace pioneer {
namespace lua {

/// Helper class with useful methods for working with semantic tree and semantic nodes.
class SemanticTreeManager
{
public:
	/// Constructor. Accepts semantic tree which this class shall wrap. It also takes a reference to a error reporter
	/// where all tree-related errors will be reported and reference to a boolean variable which will be set to true
	/// in case of errors.
	explicit SemanticTreeManager(
			generatorBase::semantics::SemanticTree &semanticTree
			, qReal::ErrorReporterInterface &errorReporter
			, bool &errorsOccured);

	/// Returns true if this node is label, i.e. synthetic node that marks other node in a tree.
	static bool isGotoNode(const generatorBase::semantics::SemanticNode * const node);

	/// Returns first non-synthetic right sibling of a given node. Returns nullptr if there is no such sibling.
	static generatorBase::semantics::SemanticNode *nonSyntheticRightSibling(
			generatorBase::semantics::SemanticNode * const node);

	/// Returns first sibling of a given node. Returns nullptr if there is no such sibling.
	static generatorBase::semantics::SemanticNode *anyRightSibling(
			generatorBase::semantics::SemanticNode * const node);

	/// Returns parent of a given node.
	static generatorBase::semantics::NonZoneNode *parent(generatorBase::semantics::SemanticNode * const node);

	/// Returns ancestor of a given node that is on a top level (not inside If branch or some other zone).
	static generatorBase::semantics::NonZoneNode *topLevelParent(generatorBase::semantics::SemanticNode * const node);

	/// Adds nextNode as a right sibling of thisNode.
	static void addAfter(generatorBase::semantics::SemanticNode * const thisNode
			, generatorBase::semantics::SemanticNode * const nextNode);

	/// Returns true if given node is a direct child of a root node.
	static bool isTopLevelNode(const generatorBase::semantics::SemanticNode * const node);

	/// Searches for a first sibling for which given predicate is true.
	static generatorBase::semantics::SemanticNode *findSibling(generatorBase::semantics::SemanticNode *node
			, std::function<bool(generatorBase::semantics::SemanticNode *)> predicate);

	/// Copies right siblings of the @p node until predicate for a sibling becomes true, or until end.
	/// @returns a list of copied nodes.
	QLinkedList<generatorBase::semantics::SemanticNode *> copyRightSiblingsUntil(
			generatorBase::semantics::SemanticNode *node
			, std::function<bool(generatorBase::semantics::SemanticNode *)> predicate);

	/// Creates a new node for a given Id, registers it as clone if node with such Id is already in semantic tree.
	generatorBase::semantics::NonZoneNode *produceNode(const qReal::Id &id);

	/// Creates node with label for a given id. Does not transfer ownership.
	generatorBase::semantics::NonZoneNode *produceLabeledNode(const qReal::Id &block);

	/// Produces a node from given block and adds it to a given zone.
	void addToZone(generatorBase::semantics::ZoneNode * const zone, const qReal::Id &id);

	/// Returns a list of nodes in a semantic tree corresponding to a given Id.
	QList<generatorBase::semantics::NonZoneNode *> nodes(const qReal::Id &id) const;

	/// Returns a list of clones for a given semantic node.
	QList<generatorBase::semantics::NonZoneNode *> clones(generatorBase::semantics::NonZoneNode *node) const;

private:
	/// Holds information about clones --- cloned node itself and a prototype of this node from which it was cloned.
	struct CloneInfo {
		generatorBase::semantics::NonZoneNode *clone;
		generatorBase::semantics::NonZoneNode *parent;
	};

	/// Creates a clone of given node. If node is compound statement, copies its children also.
	generatorBase::semantics::NonZoneNode *copy(generatorBase::semantics::NonZoneNode *node);

	/// Returns true if this node is synthetic (has no corresponding block on a diagram).
	static bool isSynthetic(const generatorBase::semantics::SemanticNode * const node);

	/// Copies children from zone node @p from to zone node @p to.
	void copyIfBranch(generatorBase::semantics::ZoneNode * const from, generatorBase::semantics::ZoneNode * const to);

	/// Registers one node as a clone of the other node.
	void registerClone(generatorBase::semantics::SemanticNode * const original
			, generatorBase::semantics::SemanticNode * const clone);

	/// Logs an error and flags that there were errors.
	void reportError(const QString &message);

	/// Reeturns true if a given node is a prototype of given clone info (maybe indirect).
	bool isParent(generatorBase::semantics::NonZoneNode *node, const CloneInfo &cloneInfo) const;

	/// Semantic tree on which we shall work.
	generatorBase::semantics::SemanticTree &mSemanticTree;

	/// Error reporter object to which to report errors.
	qReal::ErrorReporterInterface &mErrorReporter;

	/// Reference to a boolean variable which shall be set in case of errors.
	bool &mErrorsOccured;

	/// For each Id contains a list of nodes that correspond to this id in a semantic tree. Normally there shall be
	/// only one node in a tree with given id, but excessive copy-pasting done by generation algorithm complicates
	/// things, so it is needed to keep track of copied nodes.
	/// This invariant holds: for each Id there is 0 or 1 node in semantic tree corresponding to this Id, or there is
	/// 2 or more nodes corresponding to this Id in mClones.
	QMultiHash<qReal::Id, CloneInfo> mClones;
};

}
}
