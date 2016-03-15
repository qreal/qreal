/* Copyright 2016 Dmitry Mordvinov
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

#include "qrgraph/multigraph.h"

namespace qrgraph {

/// Provides implementations for some useful algorithms on multigraph.
class Queries
{
public:
	/// Traverses multigraph owning \a start by deep-first-search method. Transitions will happen only towards edges
	/// of type \a edgeType. At each visited vertex \a processor will be called. Value returned by \a processor is
	/// regarded like success: if true returned algorithm will consider that target node was found and it will be
	/// terminated returning true. If false returned by \a processor traversal will be continued.
	/// @returns True if required value was found (\a processor returned true for some node) or false otherwise.
	/// @note This implementation uses O(n) memory to memorize visited vertices. It always terminates.
	/// @see bfs(), treeLift().
	static bool dfs(const Node &start, const std::function<bool(const Node &node)> &processor, uint edgeType);

	/// Traverses multigraph owning \a start by breadth-first-search method. Transitions will happen only towards edges
	/// of type \a edgeType. At each visited vertex \a processor will be called. Value returned by \a processor is
	/// regarded like success: if true returned algorithm will consider that target node was found and it will be
	/// terminated returning true. If false returned by \a processor traversal will be continued.
	/// @returns True if required value was found (\a processor returned true for some node) or false otherwise.
	/// @note This implementation uses O(n) memory to memorize visited vertices. It always terminates.
	/// @see dfs(), treeLift().
	static bool bfs(const Node &start, const std::function<bool(const Node &node)> &processor, uint edgeType);

	/// Starting from \a start goes into first met node, calls processor, goes into first met node again and so on.
	/// Visited vertices are not memorized. Useful for quick lifting to a root of a tree, but the assumption that graph
	/// is tree is not checked at all. Transition occurs only towards the edge of \a edgeType. At each visited vertex
	/// \a processor will be called. Value returned by \a processor is regarded like success: if true returned
	/// algorithm will consider that target node was found and it will be terminated returning true. If false returned
	/// by \a processor traversal will be continued.
	/// @returns True if required value was found (\a processor returned true for some node) or false otherwise.
	/// @note Obviously if cicle met the algorithm never terminates. This is not exactly true for this implementation
	/// cause it will terminate when n vertices were visited where n is a number of vertices in multigraph. Assertion
	/// fault will be then generated.
	/// @see dfs() ands bfs() for more safe but slower traversal.
	static bool treeLift(const Node &start, const std::function<bool(const Node &node)> &processor, uint edgeType);

	/// Calls \a processor for each node reachable from \a start in one step towards edge of \a edgeType.
	/// Value returned by \a processor is regarded like success: if true returned algorithm will consider
	/// that target node was found and it will be terminated returning true. If false returned by \a processor
	/// traversal will be continued.
	/// @returns True if required value was found (\a processor returned true for some node) or false otherwise.
	static bool oneStep(const Node &start, const std::function<bool(const Node &node)> &processor, uint edgeType);

	/// Returns true if \a to vertex is reachable from \a from vertex when transiting only by edges of type \a edgeType.
	/// @note The implementation uses deep-first-search. This node is always reachable from itself.
	static bool isReachable(const Node &from, const Node &to, uint edgeType);

	/// Returns true if \a to vertex is reachable from \a from vertex using treeLift() traversal.
	/// @warning This method requires assumption that no cycles will be met (graph is a tree). If cycle is met
	/// during traversal assertion fault will be generated (although this may not happen even if graph has cycles).
	/// This node is always reachable from itself.
	static bool isReachableInTree(const Node &child, const Node &parent, uint edgeType);

	/// Returns a list of nodes reachable from \a node in one step towards edges of \a edgeType.
	/// It is guaranteed that each node will be met in resulting set not more that once.
	/// @note If loop-edges are met then \a node will be in resulting set too.
	static QList<const Node *> immediateFollowers(const Node &node, uint edgeType);

	/// Returns a list of such nodes that \a node is reachable from them in one step towards edges of \a edgeType.
	/// It is guaranteed that each node will be met in resulting set not more that once.
	/// @note If loop-edges are going out of \a node then \a node will be in resulting set too.
	static QList<const Node *> immediatePredecessors(const Node &node, uint edgeType);

	/// Returns a list of nodes reachable from \a node in any number of steps towards edges of \a edgeType.
	/// It is guaranteed that each node will be met in resulting set not more that once.
	/// @note \a node will be always included into resulting set. The implementation uses deep-first-search traversal.
	static QList<const Node *> reachableSet(const Node &node, uint edgeType);
};

}
