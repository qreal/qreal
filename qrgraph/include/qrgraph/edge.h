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

namespace qrgraph {

class Multigraph;
class Node;

/// Base class for multigraph edges. Different models parts can inherit itself from this class to
/// explicitly mark that they connect entities of some graph.
class Edge
{
public:
	/// Returns multigraph owning this instance of node.
	Multigraph &graph() const;

	/// Returns type of this edge. Types can implicitly represent some complex information, just use qHash().
	unsigned int type() const;

	/// Returns node connected to the begin of this edge. May be nullptr.
	Node *begin() const;

	/// Returns node connected to the end of this edge. May be nullptr.
	Node *end() const;

	/// Connects begin of this edge to some node.
	void connectBegin(Node &node);

	/// Connects end of this edge to some node.
	void connectEnd(Node &node);

	/// Connects begin and end of this edge to some node.
	void connect(Node &begin, Node &end);

	/// Disconnects the begin of the edge from node if it is connected.
	void disconnectBegin();

	/// Disconnects the end of the edge from node if it is connected.
	void disconnectEnd();

	/// Disconnects the edge from nodes if it is connected. The edge is still remaining hanging in multigraph.
	void disconnect();

protected:
	Edge(Multigraph &parent, unsigned int type);

	/// @warning: Do not delete edges manually, use Multigraph::removeEdge instead.
	virtual ~Edge();

private:
	Multigraph &mParent;
	unsigned int mType;
	Node *mBegin;
	Node *mEnd;

	friend class Multigraph;
};

}
