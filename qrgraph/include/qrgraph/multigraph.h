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

#include "qrgraph/node.h"
#include "qrgraph/edge.h"

namespace qrgraph {

/// Base class for all labeled multigraph entities: graph that can have multiple edges connecting same two vertices.
/// Different models can inherit itself from this class to be explicitly represented as multigraph.
/// Contains implementations for basic graph and multigraph operations.
class Multigraph
{
public:
	Multigraph();
	virtual ~Multigraph();

	/// Returns true if multigraph does contains no nodes and edges.
	bool isEmpty() const;

	/// Removes read-only list of multigraph vertices, sorted by inserting time.
	const QList<Node *> &vertices() const;

	/// Returns true if this multigraph instance has \a node vertex.
	bool containsNode(Node &node) const;

	/// Returns true if this multigraph instance has \a edge, no matter hanging or not.
	bool containsEdge(Edge &edge) const;

	/// Removes a number of vertices in multigraph.
	int verticesCount() const;

	/// Removes a number of edges of all types in multigraph.
	int edgesCount() const;

	/// Removes a number of edges of a given type in multigraph.
	int edgesCount(uint type) const;

	/// Removes all nodes and edges from multigraph, frees their memory.
	void clear();

	/// Creates and adds node into this multigraph instance. Node then can be connected to edges via Edge::connect.
	/// @note Multigraph takes ownership on result.
	virtual Node &produceNode();

	/// Appends vertex into this instance multigraph. If vertex does not belong to this multigraph or already contained
	/// by this instance nothing will happen.
	/// @note Multigraph will take ownership on \a node.
	virtual void addNode(Node &node);

	/// Creates and adds edge into this multigraph instance. Edge then can be connected to nodes via Edge::connect.
	/// @param type The type of the edge. Edges of desired types can be then conveniently filtered out from the whole
	/// set of edges outgoing or incoming into some node for all operations on this multigraph.
	/// @note This is the only way to produce edges and to add them to graph. Multigraph takes ownership on result.
	virtual Edge &produceEdge(uint type = 0);

	/// Creates and adds edge into this multigraph instance and connects its begin to the given node.
	/// @param type The type of the edge. Edges of desired types can be then conveniently filtered out from the whole
	/// set of edges outgoing or incoming into some node for all operations on this multigraph.
	/// @note This is the only way to produce edges and to add them to graph. Multigraph takes ownership on result.
	Edge &produceEdge(Node &begin, uint type = 0);

	/// Creates and adds edge into this multigraph instance and connects its begin and end to the given nodes.
	/// @param type The type of the edge. Edges of desired types can be then conveniently filtered out from the whole
	/// set of edges outgoing or incoming into some node for all operations on this multigraph.
	/// @note This is the only way to produce edges and to add them to graph. Multigraph takes ownership on result.
	Edge &produceEdge(Node &begin, Node &end, uint type = 0);

	/// Appends edge into this instance multigraph. If edge does not belong to this multigraph or already contained
	/// by this instance nothing will happen.
	/// @note Multigraph will take ownership on \a edge.
	virtual void addEdge(Edge &edge);

	/// Detaches \a node from this multigraph instance and frees its memory.
	/// @param deleteHangingEdges If true then all edges that are connected only to this node will be removed too.
	void removeNode(Node &node, bool deleteHangingEdges = false);

	/// Detaches \a edge from this multigraph instance and frees its memory.
	void removeEdge(Edge &edge);

private:
	QList<Node *> mNodes;
	QMultiHash<uint, Edge *> mEdges;
};

}
