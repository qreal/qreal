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

#include "qrgraph/queries.h"

#include <QtCore/QSet>
#include <QtCore/QQueue>

using namespace qrgraph;

bool dfs(const Node &start, const std::function<bool(const Node &node)> &processor
		, uint edgeType, QSet<const Node *> &visited)
{
	if (visited.contains(&start)) {
		return false;
	}

	visited.insert(&start);
	if (processor(start)) {
		// Target is found, search succeeded.
		return true;
	}

	auto goNext = [&processor, edgeType, &visited](const Node &node) {
		return dfs(node, processor, edgeType, visited);
	};

	return Queries::oneStep(start, goNext, edgeType);
}

bool Queries::dfs(const Node &start, const std::function<bool (const Node &)> &processor, uint edgeType)
{
	QSet<const Node *> visited;
	return ::dfs(start, processor, edgeType, visited);
}

bool Queries::bfs(const Node &start, const std::function<bool (const Node &)> &processor, uint edgeType)
{
	QSet<const Node *> visited;
	QQueue<const Node *> queue;
	queue.enqueue(&start);

	while (!queue.isEmpty()) {
		const Node *node = queue.dequeue();
		visited.insert(node);
		if (processor(*node)) {
			// Target is found, search succeeded.
			return true;
		}

		const QList<Edge *> edges = node->outgoingEdges(edgeType);
		for (const Edge *edge : edges) {
			if (edge->end() && !visited.contains(edge->end())) {
				queue.enqueue(edge->end());
			}
		}
	}

	// We found nothing, this the whole search was not sucessful.
	return false;
}

bool Queries::treeLift(const Node &start, const std::function<bool (const Node &)> &processor, uint edgeType)
{
	const Node *currentNode = &start;
	int stepsRemained = start.graph().verticesCount();
	while (currentNode && stepsRemained > 0) {
		if (processor(*currentNode)) {
			return true;
		}

		const QList<Edge *> edges = currentNode->outgoingEdges(edgeType);
		currentNode = nullptr;
		for (const Edge *edge : edges) {
			currentNode = edge->end();
			if (currentNode) {
				--stepsRemained;
				break;
			}
		}
	}

	Q_ASSERT_X(stepsRemained > 0, Q_FUNC_INFO, "Steps limit exceeded, stuck in a cycle.");
	return false;
}

bool Queries::oneStep(const Node &start, const std::function<bool (const Node &)> &processor, uint edgeType)
{
	const QList<Edge *> edges = start.outgoingEdges(edgeType);
	for (const Edge *edge : edges) {
		if (edge->end() && processor(*edge->end())) {
			return true;
		}
	}

	return false;
}

bool Queries::isReachable(const Node &from, const Node &to, uint edgeType)
{
	return dfs(from, [&to](const Node &node) {return &node == &to;}, edgeType);
}

bool Queries::isReachableInTree(const Node &from, const Node &to, uint edgeType)
{
	return treeLift(from, [&to](const Node &node) {return &node == &to;}, edgeType);
}

QList<const Node *> Queries::immediateFollowers(const Node &node, uint edgeType)
{
	QSet<const Node *> result;
	const QList<Edge *> edges = node.outgoingEdges(edgeType);
	for (const Edge *edge : edges) {
		if (edge->end()) {
			result.insert(edge->end());
		}
	}

	return result.toList();
}

QList<const Node *> Queries::immediatePredecessors(const Node &node, uint edgeType)
{
	QSet<const Node *> result;
	const QList<Edge *> edges = node.incomingEdges(edgeType);
	for (const Edge *edge : edges) {
		if (edge->begin()) {
			result.insert(edge->begin());
		}
	}

	return result.toList();
}

QList<const Node *> Queries::reachableSet(const Node &node, uint edgeType)
{
	QSet<const Node *> result;
	::dfs(node, [](const Node &) {return false;}, edgeType, result);
	return result.toList();
}
