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

#include <QtCore/QMultiHash>

namespace qrgraph {

class Multigraph;
class Edge;

/// Base class for multigraph nodes. Different models parts can inherit itself from this class to
/// explicitly mark that they represent entities of some graph.
class Node
{
public:
	/// Returns multigraph owning this instance of node.
	Multigraph &graph() const;

	/// Returns a list of all edges outgoing from this node, in an arbitrary order.
	QList<Edge *> outgoingEdges() const;

	/// Returns a number of all edges outgoing from this node.
	int outgoingEdgesCount() const;

	/// Returns a list of edges of type \a type outgoing from this node, in arbitrary order.
	QList<Edge *> outgoingEdges(uint type) const;

	/// Returns a number of edges of type \a type outgoing from this node.
	int outgoingEdgesCount(uint type) const;

	/// Returns a list of all edges incoming into this node, in an arbitrary order.
	QList<Edge *> incomingEdges() const;

	/// Returns a number of all edges incoming into this node.
	int incomingEdgesCount() const;

	/// Returns a list of edges of type \a type incoming into this node, in arbitrary order.
	QList<Edge *> incomingEdges(uint type) const;

	/// Returns a number of edges of type \a type incoming into this node.
	int incomingEdgesCount(uint type) const;

protected:
	explicit Node(Multigraph &parent);

	/// @warning: Do not delete nodes manually, use Multigraph::removeNode instead.
	virtual ~Node();

	/// Disconnects all outgoing edges from this node.
	/// @param deleteHangingEdges If true then all edges connected only to this node will be deleted from multigraph.
	void disconnectOutgoing(bool deleteHangingEdges);

	/// Disconnects all incoming edges from this node.
	/// @param deleteHangingEdges If true then all edges connected only to this node will be deleted from multigraph.
	void disconnectIncoming(bool deleteHangingEdges);

	/// Disconnects outgoing and incoming edges from this node.
	/// @param deleteHangingEdges If true then all edges connected only to this node will be deleted from multigraph.
	void disconnectAll(bool deleteHangingEdges);

private:
	/// This will be called by friend Edge class when this Node was connected to the begin of \a edge.
	/// Adds edge to the list of outgoing edges on the port corresponding to edge's type.
	void connectBeginOf(Edge &edge);

	/// This will be called by friend Edge class when this Node was connected to the end of \a edge.
	/// Adds edge to the list of incoming edges on the port corresponding to edge's type.
	void connectEndOf(Edge &edge);

	/// This will be called by friend Edge class when this Node was disconnected from the begin of \a edge.
	/// Removes edge from the list of outgoing edges on the port corresponding to edge's type.
	void disconnectBeginOf(Edge &edge);

	/// This will be called by friend Edge class when this Node was disconnected from the end of \a edge.
	/// Removes edge from the list of incoming edges on the port corresponding to edge's type.
	void disconnectEndOf(Edge &edge);

	Multigraph &mParent;
	QMultiHash<uint, Edge *> mOutgoingEdges;
	QMultiHash<uint, Edge *> mIncomingEdges;

	friend class qrgraph::Edge;
	friend class qrgraph::Multigraph;
};

}
