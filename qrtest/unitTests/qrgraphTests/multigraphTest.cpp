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

#include "multigraphTest.h"

using namespace qrTest;
using namespace qrgraph;

void MultigraphTest::setUpCase1(Multigraph &graph)
{
	// Sets up the directed multigraph drawn below. Use monospace font to see it! Capital letters mean vertices.
	// "--" and "~~" are different types of edges, "-~-~-" means that vertices are connected with both types of edges.
	// "[" near the vertex means loop of the first type, "c" means loop of the second type.
	// Edges are oriented from lexicographically less to lexicographically greater letter.
	//
	//              A-~-~-~-B
	//               ~     /
	//                ~   /
	//                 ~ /
	//                 [C
	//                 / ~
	//                /   ~
	//               /     ~
	//             cD-~-~-~-E

	Node &nodeA = graph.produceNode();
	Node &nodeB = graph.produceNode();
	Node &nodeC = graph.produceNode();
	Node &nodeD = graph.produceNode();
	Node &nodeE = graph.produceNode();

	graph.produceEdge(nodeA, nodeB, 0);
	graph.produceEdge(nodeA, nodeB, 1);
	graph.produceEdge(nodeA, nodeC, 1);
	graph.produceEdge(nodeB, nodeC, 0);
	graph.produceEdge(nodeC, nodeC, 0);
	graph.produceEdge(nodeC, nodeD, 0);
	graph.produceEdge(nodeC, nodeE, 1);
	graph.produceEdge(nodeD, nodeD, 1);
	graph.produceEdge(nodeD, nodeE, 0);
	graph.produceEdge(nodeD, nodeE, 1);
}

void MultigraphTest::setUpCase2(Multigraph &graph)
{
	// Sets up the directed multigraph drawn below. Use monospace font to see it! Capital letters mean vertices.
	// "--" and "~~" are different types of edges, "-~-~-" means that vertices are connected with both types of edges.
	// "[" or "]" near the vertex means loop of the first type, "c" means loop of the second type.
	// Edges are oriented from lexicographically less to lexicographically greater letter.
	//
	//              A-~-~-~-B                       cE-~-~-~-F]
	//               ~     //                        ~~
	//                ~   //       cccD]]             ~~
	//                 ~ //                  H         ~~
	//                 cC]                              cG]

	Node &nodeA = graph.produceNode();
	Node &nodeB = graph.produceNode();
	Node &nodeC = graph.produceNode();
	Node &nodeD = graph.produceNode();
	Node &nodeE = graph.produceNode();
	Node &nodeF = graph.produceNode();
	Node &nodeG = graph.produceNode();
	Node &nodeH = graph.produceNode();
	Q_UNUSED(nodeH)

	graph.produceEdge(nodeA, nodeB, 0);
	graph.produceEdge(nodeA, nodeB, 1);
	graph.produceEdge(nodeA, nodeC, 1);
	graph.produceEdge(nodeB, nodeC, 0);
	graph.produceEdge(nodeB, nodeC, 0);
	graph.produceEdge(nodeC, nodeC, 0);
	graph.produceEdge(nodeC, nodeC, 1);

	graph.produceEdge(nodeD, nodeD, 0);
	graph.produceEdge(nodeD, nodeD, 0);
	graph.produceEdge(nodeD, nodeD, 1);
	graph.produceEdge(nodeD, nodeD, 1);
	graph.produceEdge(nodeD, nodeD, 1);

	graph.produceEdge(nodeE, nodeE, 1);
	graph.produceEdge(nodeE, nodeF, 0);
	graph.produceEdge(nodeE, nodeF, 1);
	graph.produceEdge(nodeE, nodeG, 1);
	graph.produceEdge(nodeE, nodeG, 1);
	graph.produceEdge(nodeF, nodeF, 0);
	graph.produceEdge(nodeG, nodeG, 0);
	graph.produceEdge(nodeG, nodeG, 1);
}

void MultigraphTest::checkCase1(Multigraph &graph)
{
	// Asserts multigraph is exactly setted up in case 1
	ASSERT_FALSE(graph.isEmpty());
	ASSERT_EQ(graph.verticesCount(), 5);
	ASSERT_EQ(graph.edgesCount(), 10);

	const Node &nodeA = *graph.vertices()[0];
	const Node &nodeB = *graph.vertices()[1];
	const Node &nodeC = *graph.vertices()[2];
	const Node &nodeD = *graph.vertices()[3];
	const Node &nodeE = *graph.vertices()[4];

	checkVertex(graph, nodeA, {&nodeB}, {&nodeB, &nodeC}, {}, {});
	checkVertex(graph, nodeB, {&nodeC}, {}, {&nodeA}, {&nodeA});
	checkVertex(graph, nodeC, {&nodeC, &nodeD}, {&nodeE}, {&nodeB, &nodeC}, {&nodeA});
	checkVertex(graph, nodeD, {&nodeE}, {&nodeD, &nodeE}, {&nodeC}, {&nodeD});
	checkVertex(graph, nodeE, {}, {}, {&nodeD}, {&nodeC, &nodeD});
}

void MultigraphTest::checkCase2(Multigraph &graph)
{
	// Asserts multigraph is exactly setted up in case 2
	ASSERT_FALSE(graph.isEmpty());
	ASSERT_EQ(graph.verticesCount(), 8);
	ASSERT_EQ(graph.edgesCount(), 20);

	const Node &nodeA = *graph.vertices()[0];
	const Node &nodeB = *graph.vertices()[1];
	const Node &nodeC = *graph.vertices()[2];
	const Node &nodeD = *graph.vertices()[3];
	const Node &nodeE = *graph.vertices()[4];
	const Node &nodeF = *graph.vertices()[5];
	const Node &nodeG = *graph.vertices()[6];
	const Node &nodeH = *graph.vertices()[7];

	checkVertex(graph, nodeA, {&nodeB}, {&nodeB, &nodeC}, {}, {});
	checkVertex(graph, nodeB, {&nodeC, &nodeC}, {}, {&nodeA}, {&nodeA});
	checkVertex(graph, nodeC, {&nodeC}, {&nodeC}, {&nodeB, &nodeB, &nodeC}, {&nodeA, &nodeC});
	checkVertex(graph, nodeD, {&nodeD, &nodeD}, {&nodeD, &nodeD, &nodeD}, {&nodeD, &nodeD}, {&nodeD, &nodeD, &nodeD});
	checkVertex(graph, nodeE, {&nodeF}, {&nodeE, &nodeF, &nodeG, &nodeG}, {}, {&nodeE});
	checkVertex(graph, nodeF, {&nodeF}, {}, {&nodeE, &nodeF}, {&nodeE});
	checkVertex(graph, nodeG, {&nodeG}, {&nodeG}, {&nodeG}, {&nodeE, &nodeE, &nodeG});
	checkVertex(graph, nodeH, {}, {}, {}, {});
}

void MultigraphTest::checkVertex(Multigraph &graph, const Node &node
		, QList<const Node *> outgoingNodesType0
		, QList<const Node *> outgoingNodesType1
		, QList<const Node *> incomingNodesType0
		, QList<const Node *> incomingNodesType1)
{
	ASSERT_EQ(&graph, &node.graph());

	ASSERT_EQ(node.outgoingEdgesCount(), outgoingNodesType0.count() + outgoingNodesType1.count());
	ASSERT_EQ(node.outgoingEdgesCount(0), outgoingNodesType0.count());
	ASSERT_EQ(node.outgoingEdgesCount(1), outgoingNodesType1.count());
	ASSERT_EQ(node.incomingEdgesCount(), incomingNodesType0.count() + incomingNodesType1.count());
	ASSERT_EQ(node.incomingEdgesCount(0), incomingNodesType0.count());
	ASSERT_EQ(node.incomingEdgesCount(1), incomingNodesType1.count());

	const QList<Edge *> outgoing0 = node.outgoingEdges(0);
	const QList<Edge *> outgoing1 = node.outgoingEdges(1);
	const QList<Edge *> incoming0 = node.incomingEdges(0);
	const QList<Edge *> incoming1 = node.incomingEdges(1);

	ASSERT_EQ(outgoing0.size(), outgoingNodesType0.size());
	ASSERT_EQ(outgoing1.size(), outgoingNodesType1.size());
	ASSERT_EQ(incoming0.size(), incomingNodesType0.size());
	ASSERT_EQ(incoming1.size(), incomingNodesType1.size());

	QList<const Edge *> loops0;
	QList<const Edge *> loops1;

	for (const Edge *edge : outgoing0) {
		ASSERT_TRUE(outgoingNodesType0.contains(edge->end()));
		outgoingNodesType0.removeOne(edge->end());
		if (edge->end() == &node) {
			loops0 << edge;
		}
	}

	for (const Edge *edge : outgoing1) {
		ASSERT_TRUE(outgoingNodesType1.contains(edge->end()));
		outgoingNodesType1.removeOne(edge->end());
		if (edge->end() == &node) {
			loops1 << edge;
		}
	}

	for (const Edge *edge : incoming0) {
		ASSERT_TRUE(incomingNodesType0.contains(edge->begin()));
		incomingNodesType0.removeOne(edge->begin());
		if (edge->begin() == &node) {
			ASSERT_TRUE(loops0.contains(edge));
			loops0.removeAll(edge);
		}
	}

	for (const Edge *edge : incoming1) {
		ASSERT_TRUE(incomingNodesType1.contains(edge->begin()));
		incomingNodesType1.removeOne(edge->begin());
		if (edge->begin() == &node) {
			ASSERT_TRUE(loops1.contains(edge));
			loops1.removeAll(edge);
		}
	}

	ASSERT_TRUE(outgoingNodesType0.isEmpty());
	ASSERT_TRUE(outgoingNodesType1.isEmpty());
	ASSERT_TRUE(incomingNodesType0.isEmpty());
	ASSERT_TRUE(incomingNodesType1.isEmpty());

	ASSERT_TRUE(loops0.isEmpty());
	ASSERT_TRUE(loops1.isEmpty());
}

TEST_F(MultigraphTest, smokeTest)
{
	Multigraph graph1, graph2;
	ASSERT_TRUE(graph1.isEmpty());
	ASSERT_TRUE(graph1.isEmpty());

	setUpCase1(graph1);
	checkCase1(graph1);
	setUpCase2(graph2);
	checkCase2(graph2);
}

TEST_F(MultigraphTest, reusabilityTest)
{
	Multigraph graph;

	setUpCase1(graph);
	checkCase1(graph);

	graph.clear();
	ASSERT_TRUE(graph.isEmpty());

	setUpCase2(graph);
	checkCase2(graph);
}

TEST_F(MultigraphTest, removingTest)
{
	Multigraph graph;
	setUpCase1(graph);
	checkCase1(graph);

	Node &nodeA = *graph.vertices()[0];
	Node &nodeB = *graph.vertices()[1];
	Node &nodeC = *graph.vertices()[2];
	Node &nodeD = *graph.vertices()[3];
	Node &nodeE = *graph.vertices()[4];

	Edge &loopD = *nodeD.incomingEdges(1).first();

	graph.removeNode(nodeD, false);
	graph.removeNode(nodeE, true);

	ASSERT_TRUE(graph.containsEdge(loopD));
	graph.removeEdge(loopD);

	ASSERT_FALSE(graph.isEmpty());
	ASSERT_EQ(graph.verticesCount(), 3);
	ASSERT_EQ(graph.edgesCount(), 7);
	checkVertex(graph, nodeA, {&nodeB}, {&nodeB, &nodeC}, {}, {});
	checkVertex(graph, nodeB, {&nodeC}, {}, {&nodeA}, {&nodeA});
	checkVertex(graph, nodeC, {&nodeC, nullptr}, {nullptr}, {&nodeB, &nodeC}, {&nodeA});

	graph.removeNode(nodeC, true);

	ASSERT_FALSE(graph.isEmpty());
	ASSERT_EQ(graph.verticesCount(), 2);
	ASSERT_EQ(graph.edgesCount(), 4);
	checkVertex(graph, nodeA, {&nodeB}, {&nodeB, nullptr}, {}, {});
	checkVertex(graph, nodeB, {nullptr}, {}, {&nodeA}, {&nodeA});

	graph.removeNode(nodeA, true);

	ASSERT_FALSE(graph.isEmpty());
	ASSERT_EQ(graph.verticesCount(), 1);
	ASSERT_EQ(graph.edgesCount(), 3);
	checkVertex(graph, nodeB, {nullptr}, {}, {nullptr}, {nullptr});

	graph.removeNode(nodeB, true);
	ASSERT_EQ(graph.verticesCount(), 0);
	ASSERT_EQ(graph.edgesCount(), 0);

	// Checking reusablilty after all
	setUpCase2(graph);
	checkCase2(graph);
}
