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

#include <qrgraph/queries.h>
#include "multigraphTest.h"

using namespace qrTest;
using namespace qrgraph;

TEST(QueriesTest, dfsTest)
{
	Multigraph graph;
	MultigraphTest::setUpCase1(graph);

	const Node &nodeA = *graph.vertices()[0];
	const Node &nodeB = *graph.vertices()[1];
	const Node &nodeC = *graph.vertices()[2];
	const Node &nodeD = *graph.vertices()[3];
	const Node &nodeE = *graph.vertices()[4];

	QList<const Node *> visited;
	auto appendHandler = [&visited](const Node &node) {
		visited << &node;
		return false;
	};

	ASSERT_FALSE(Queries::dfs(nodeA, appendHandler, 1));
	ASSERT_EQ(visited.count(), 4);
	ASSERT_TRUE(visited.contains(&nodeA));
	ASSERT_TRUE(visited.contains(&nodeB));
	ASSERT_TRUE(visited.contains(&nodeC));
	ASSERT_FALSE(visited.contains(&nodeD));
	ASSERT_TRUE(visited.contains(&nodeE));

	visited.clear();
	ASSERT_FALSE(Queries::dfs(nodeA, appendHandler, 0));
	ASSERT_EQ(visited.count(), 5);
	ASSERT_TRUE(visited.contains(&nodeA));
	ASSERT_TRUE(visited.contains(&nodeB));
	ASSERT_TRUE(visited.contains(&nodeC));
	ASSERT_TRUE(visited.contains(&nodeD));
	ASSERT_TRUE(visited.contains(&nodeE));

	auto appendAndTerminateOnCHandler = [&visited, &nodeC](const Node &node) {
		visited << &node;
		return &node == &nodeC;
	};

	visited.clear();
	ASSERT_TRUE(Queries::dfs(nodeA, appendAndTerminateOnCHandler, 0));
	ASSERT_TRUE(visited.count() == 2 || visited.count() == 3);
	ASSERT_TRUE(visited.contains(&nodeA));
	ASSERT_TRUE(visited.contains(&nodeC));
	ASSERT_FALSE(visited.contains(&nodeD));
	ASSERT_FALSE(visited.contains(&nodeE));

	visited.clear();
	ASSERT_TRUE(Queries::dfs(nodeA, appendAndTerminateOnCHandler, 1));
	ASSERT_TRUE(visited.count() == 2 || visited.count() == 3);
	ASSERT_TRUE(visited.contains(&nodeA));
	ASSERT_TRUE(visited.contains(&nodeC));
	ASSERT_FALSE(visited.contains(&nodeD));
	ASSERT_FALSE(visited.contains(&nodeE));
}

TEST(QueriesTest, bfsTest)
{
	Multigraph graph;
	MultigraphTest::setUpCase1(graph);

	const Node &nodeA = *graph.vertices()[0];
	const Node &nodeB = *graph.vertices()[1];
	const Node &nodeC = *graph.vertices()[2];
	const Node &nodeD = *graph.vertices()[3];
	const Node &nodeE = *graph.vertices()[4];

	QList<const Node *> visited;
	auto appendHandler = [&visited](const Node &node) {
		visited << &node;
		return false;
	};

	ASSERT_FALSE(Queries::bfs(nodeA, appendHandler, 1));
	ASSERT_EQ(visited.count(), 4);
	ASSERT_TRUE(visited.contains(&nodeA));
	ASSERT_TRUE(visited.contains(&nodeB));
	ASSERT_TRUE(visited.contains(&nodeC));
	ASSERT_FALSE(visited.contains(&nodeD));
	ASSERT_TRUE(visited.contains(&nodeE));

	visited.clear();
	ASSERT_FALSE(Queries::bfs(nodeA, appendHandler, 0));
	ASSERT_EQ(visited.count(), 5);
	ASSERT_TRUE(visited.contains(&nodeA));
	ASSERT_TRUE(visited.contains(&nodeB));
	ASSERT_TRUE(visited.contains(&nodeC));
	ASSERT_TRUE(visited.contains(&nodeD));
	ASSERT_TRUE(visited.contains(&nodeE));

	auto appendAndTerminateOnCHandler = [&visited, &nodeC](const Node &node) {
		visited << &node;
		return &node == &nodeC;
	};

	visited.clear();
	ASSERT_TRUE(Queries::bfs(nodeA, appendAndTerminateOnCHandler, 0));
	ASSERT_TRUE(visited.count() == 2 || visited.count() == 3);
	ASSERT_TRUE(visited.contains(&nodeA));
	ASSERT_TRUE(visited.contains(&nodeC));
	ASSERT_FALSE(visited.contains(&nodeD));
	ASSERT_FALSE(visited.contains(&nodeE));

	visited.clear();
	ASSERT_TRUE(Queries::bfs(nodeA, appendAndTerminateOnCHandler, 1));
	ASSERT_TRUE(visited.count() == 2 || visited.count() == 3);
	ASSERT_TRUE(visited.contains(&nodeA));
	ASSERT_TRUE(visited.contains(&nodeC));
	ASSERT_FALSE(visited.contains(&nodeD));
	ASSERT_FALSE(visited.contains(&nodeE));
}

TEST(QueriesTest, treeLiftTest)
{
	Multigraph graph;
	MultigraphTest::setUpCase2(graph);

	const Node &nodeA = *graph.vertices()[0];
	const Node &nodeB = *graph.vertices()[1];
	const Node &nodeC = *graph.vertices()[2];
	const Node &nodeD = *graph.vertices()[3];
	const Node &nodeE = *graph.vertices()[4];
	const Node &nodeF = *graph.vertices()[5];
	const Node &nodeG = *graph.vertices()[6];
	const Node &nodeH = *graph.vertices()[7];

	for (const Node *node : graph.vertices()) {
		ASSERT_TRUE(Queries::isReachableInTree(*node, *node, 0));
		ASSERT_TRUE(Queries::isReachableInTree(*node, *node, 1));
	}

	ASSERT_TRUE(Queries::isReachableInTree(nodeA, nodeB, 0));
	ASSERT_FALSE(Queries::isReachableInTree(nodeB, nodeC, 1));
	ASSERT_FALSE(Queries::isReachableInTree(nodeB, nodeD, 1));
	ASSERT_TRUE(Queries::isReachableInTree(nodeE, nodeF, 0));
	ASSERT_FALSE(Queries::isReachableInTree(nodeF, nodeG, 1));
	ASSERT_FALSE(Queries::isReachableInTree(nodeH, nodeA, 0));
	ASSERT_FALSE(Queries::isReachableInTree(nodeH, nodeE, 1));
}

TEST(QueriesTest, reachabilityTest)
{
	Multigraph graph;
	MultigraphTest::setUpCase2(graph);

	const Node &nodeA = *graph.vertices()[0];
	const Node &nodeB = *graph.vertices()[1];
	const Node &nodeC = *graph.vertices()[2];
	const Node &nodeD = *graph.vertices()[3];
	const Node &nodeE = *graph.vertices()[4];
	const Node &nodeF = *graph.vertices()[5];
	const Node &nodeG = *graph.vertices()[6];
	const Node &nodeH = *graph.vertices()[7];

	for (const Node *node : graph.vertices()) {
		ASSERT_TRUE(Queries::isReachable(*node, *node, 0));
		ASSERT_TRUE(Queries::isReachable(*node, *node, 1));
	}

	ASSERT_TRUE(Queries::isReachable(nodeA, nodeB, 0));
	ASSERT_TRUE(Queries::isReachable(nodeA, nodeB, 1));
	ASSERT_TRUE(Queries::isReachable(nodeA, nodeC, 0));
	ASSERT_TRUE(Queries::isReachable(nodeA, nodeC, 1));
	ASSERT_TRUE(Queries::isReachable(nodeB, nodeC, 0));
	ASSERT_FALSE(Queries::isReachable(nodeB, nodeC, 1));
	ASSERT_FALSE(Queries::isReachable(nodeC, nodeA, 0));
	ASSERT_FALSE(Queries::isReachable(nodeC, nodeA, 1));
	ASSERT_FALSE(Queries::isReachable(nodeC, nodeB, 0));
	ASSERT_FALSE(Queries::isReachable(nodeC, nodeB, 1));
	ASSERT_FALSE(Queries::isReachable(nodeB, nodeA, 0));
	ASSERT_FALSE(Queries::isReachable(nodeB, nodeA, 1));

	ASSERT_TRUE(Queries::isReachable(nodeE, nodeF, 0));
	ASSERT_TRUE(Queries::isReachable(nodeE, nodeF, 1));
	ASSERT_FALSE(Queries::isReachable(nodeE, nodeG, 0));
	ASSERT_TRUE(Queries::isReachable(nodeE, nodeG, 1));
	ASSERT_FALSE(Queries::isReachable(nodeF, nodeG, 0));
	ASSERT_FALSE(Queries::isReachable(nodeF, nodeG, 1));
	ASSERT_FALSE(Queries::isReachable(nodeG, nodeE, 0));
	ASSERT_FALSE(Queries::isReachable(nodeG, nodeE, 1));
	ASSERT_FALSE(Queries::isReachable(nodeG, nodeF, 0));
	ASSERT_FALSE(Queries::isReachable(nodeG, nodeF, 1));
	ASSERT_FALSE(Queries::isReachable(nodeF, nodeE, 0));
	ASSERT_FALSE(Queries::isReachable(nodeF, nodeE, 1));

	ASSERT_FALSE(Queries::isReachable(nodeA, nodeD, 0));
	ASSERT_FALSE(Queries::isReachable(nodeA, nodeD, 1));
	ASSERT_FALSE(Queries::isReachable(nodeA, nodeH, 0));
	ASSERT_FALSE(Queries::isReachable(nodeA, nodeH, 1));
	ASSERT_FALSE(Queries::isReachable(nodeH, nodeA, 0));
	ASSERT_FALSE(Queries::isReachable(nodeH, nodeA, 1));
	ASSERT_FALSE(Queries::isReachable(nodeH, nodeA, 0));
	ASSERT_FALSE(Queries::isReachable(nodeH, nodeA, 1));
	ASSERT_FALSE(Queries::isReachable(nodeE, nodeD, 0));
	ASSERT_FALSE(Queries::isReachable(nodeE, nodeD, 1));
	ASSERT_FALSE(Queries::isReachable(nodeE, nodeH, 0));
	ASSERT_FALSE(Queries::isReachable(nodeE, nodeH, 1));
	ASSERT_FALSE(Queries::isReachable(nodeH, nodeE, 0));
	ASSERT_FALSE(Queries::isReachable(nodeH, nodeE, 1));
	ASSERT_FALSE(Queries::isReachable(nodeH, nodeE, 0));
	ASSERT_FALSE(Queries::isReachable(nodeH, nodeE, 1));
	ASSERT_FALSE(Queries::isReachable(nodeD, nodeH, 0));
	ASSERT_FALSE(Queries::isReachable(nodeD, nodeH, 1));
	ASSERT_FALSE(Queries::isReachable(nodeH, nodeD, 0));
	ASSERT_FALSE(Queries::isReachable(nodeH, nodeD, 1));
}

TEST(QueriesTest, reachabilitySetsTest)
{
	Multigraph graph;
	MultigraphTest::setUpCase1(graph);

	const Node &nodeA = *graph.vertices()[0];
	const Node &nodeB = *graph.vertices()[1];
	const Node &nodeC = *graph.vertices()[2];
	const Node &nodeD = *graph.vertices()[3];
	const Node &nodeE = *graph.vertices()[4];

	QList<const Node *> set = Queries::immediateFollowers(nodeA, 0);
	ASSERT_EQ(set.count(), 1);
	ASSERT_EQ(set.first(), &nodeB);

	set = Queries::immediateFollowers(nodeC, 0);
	ASSERT_EQ(set.count(), 2);
	ASSERT_TRUE(set.contains(&nodeC));
	ASSERT_TRUE(set.contains(&nodeD));

	set = Queries::immediatePredecessors(nodeC, 0);
	ASSERT_EQ(set.count(), 2);
	ASSERT_TRUE(set.contains(&nodeC));
	ASSERT_TRUE(set.contains(&nodeB));

	set = Queries::reachableSet(nodeA, 0);
	ASSERT_EQ(set.count(), 5);
	ASSERT_TRUE(set.contains(&nodeA));
	ASSERT_TRUE(set.contains(&nodeB));
	ASSERT_TRUE(set.contains(&nodeC));
	ASSERT_TRUE(set.contains(&nodeD));
	ASSERT_TRUE(set.contains(&nodeE));

	set = Queries::reachableSet(nodeA, 1);
	ASSERT_EQ(set.count(), 4);
	ASSERT_TRUE(set.contains(&nodeA));
	ASSERT_TRUE(set.contains(&nodeB));
	ASSERT_TRUE(set.contains(&nodeC));
	ASSERT_FALSE(set.contains(&nodeD));
	ASSERT_TRUE(set.contains(&nodeE));

	set = Queries::reachableSet(nodeB, 0);
	ASSERT_EQ(set.count(), 4);
	ASSERT_FALSE(set.contains(&nodeA));
	ASSERT_TRUE(set.contains(&nodeB));
	ASSERT_TRUE(set.contains(&nodeC));
	ASSERT_TRUE(set.contains(&nodeD));
	ASSERT_TRUE(set.contains(&nodeE));

	set = Queries::reachableSet(nodeB, 1);
	ASSERT_EQ(set.count(), 1);
	ASSERT_EQ(set.first(), &nodeB);

	set = Queries::reachableSet(nodeC, 0);
	ASSERT_EQ(set.count(), 3);
	ASSERT_FALSE(set.contains(&nodeA));
	ASSERT_FALSE(set.contains(&nodeB));
	ASSERT_TRUE(set.contains(&nodeC));
	ASSERT_TRUE(set.contains(&nodeD));
	ASSERT_TRUE(set.contains(&nodeE));

	set = Queries::reachableSet(nodeC, 1);
	ASSERT_EQ(set.count(), 2);
	ASSERT_FALSE(set.contains(&nodeA));
	ASSERT_FALSE(set.contains(&nodeB));
	ASSERT_TRUE(set.contains(&nodeC));
	ASSERT_FALSE(set.contains(&nodeD));
	ASSERT_TRUE(set.contains(&nodeE));
}
