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

#include <generatorBase/semanticTree/semanticTree.h>

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
	static bool isLabel(const generatorBase::semantics::SemanticNode * const node);

	/// Returns first non-synthetic right sibling of a given node. Returns nullptr if there is no such sibling.
	static generatorBase::semantics::SemanticNode *findRightSibling(
			generatorBase::semantics::SemanticNode * const node);

	/// Returns parent of a given node.
	static generatorBase::semantics::NonZoneNode *parent(generatorBase::semantics::SemanticNode * const node);

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

	/// Creates node with label for a given id. Does not transfer ownership.
	generatorBase::semantics::NonZoneNode *produceLabeledNode(const qReal::Id &block);

	/// Produces a node from given block and adds it to a given zone.
	void addToZone(generatorBase::semantics::ZoneNode * const zone, const qReal::Id &id);

private:
	/// Semantic tree on which we shall work.
	generatorBase::semantics::SemanticTree &mSemanticTree;

	/// Error reporter object to which to report errors.
	qReal::ErrorReporterInterface &mErrorReporter;

	/// Reference to a boolean variable which shall be set in case of errors.
	bool &mErrorsOccured;
};

}
}
