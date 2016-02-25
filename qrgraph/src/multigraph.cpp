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

#include "qrgraph/multigraph.h"

using namespace qrgraph;

Multigraph::Multigraph()
{
}

Multigraph::~Multigraph()
{
	clear();
}

bool Multigraph::isEmpty() const
{
	return mNodes.isEmpty() && mEdges.isEmpty();
}

const QList<Node *> &Multigraph::vertices() const
{
	return mNodes;
}

bool Multigraph::containsNode(Node &node) const
{
	return mNodes.contains(&node);
}

bool Multigraph::containsEdge(Edge &edge) const
{
	return mEdges.contains(edge.type(), &edge);
}

int Multigraph::verticesCount() const
{
	return mNodes.count();
}

int Multigraph::edgesCount() const
{
	return mEdges.size();
}

int Multigraph::edgesCount(uint type) const
{
	return mEdges.count(type);
}

void Multigraph::clear()
{
	// qDeleteAll cannot be used here because Node and Edge types have protected desctuctor.
	for (const Edge *edge : mEdges) {
		delete edge;
	}

	for (const Node *node : mNodes) {
		delete node;
	}

	mNodes.clear();
	mEdges.clear();
}

Node &Multigraph::produceNode()
{
	Node * const node = new Node(*this);
	mNodes << node;
	return *node;
}

Edge &Multigraph::produceEdge(uint type)
{
	Edge * const edge = new Edge(*this, type);
	mEdges.insert(type, edge);
	return *edge;
}

Edge &Multigraph::produceEdge(Node &begin, uint type)
{
	Edge &result = produceEdge(type);
	result.connectBegin(begin);
	return result;
}

Edge &Multigraph::produceEdge(Node &begin, Node &end, uint type)
{
	Edge &result = produceEdge(type);
	result.connect(begin, end);
	return result;
}

void Multigraph::removeNode(Node &node, bool deleteHangingEdges)
{
	Q_ASSERT_X(mNodes.contains(&node), Q_FUNC_INFO, "Attepmt to remove nonexisting node");
	node.disconnectAll(deleteHangingEdges);
	mNodes.removeAll(&node);
	delete &node;
}

void Multigraph::removeEdge(Edge &edge)
{
	Q_ASSERT_X(mEdges.contains(edge.type(), &edge), Q_FUNC_INFO, "Attepmt to remove nonexisting edge");
	mEdges.remove(edge.type(), &edge);
	delete &edge;
}
