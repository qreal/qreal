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

#include "qrgraph/node.h"

#include "qrgraph/multigraph.h"
#include "qrgraph/edge.h"

using namespace qrgraph;

Node::Node(Multigraph &parent)
	: mParent(parent)
{
}

Node::~Node()
{
	disconnectAll(false);
}

Multigraph &Node::graph() const
{
	return mParent;
}

QList<Edge *> Node::outgoingEdges() const
{
	return mOutgoingEdges.values();
}

int Node::outgoingEdgesCount() const
{
	return mOutgoingEdges.size();
}

QList<Edge *> Node::outgoingEdges(uint type) const
{
	return mOutgoingEdges.values(type);
}

int Node::outgoingEdgesCount(uint type) const
{
	return mOutgoingEdges.count(type);
}

QList<Edge *> Node::incomingEdges() const
{
	return mIncomingEdges.values();
}

int Node::incomingEdgesCount() const
{
	return mIncomingEdges.size();
}

QList<Edge *> Node::incomingEdges(uint type) const
{
	return mIncomingEdges.values(type);
}

int Node::incomingEdgesCount(uint type) const
{
	return mIncomingEdges.count(type);
}

void Node::connectBeginOf(Edge &edge)
{
	Q_ASSERT_X(!mOutgoingEdges.contains(edge.type(), &edge), Q_FUNC_INFO, "Edge begin is already connected");
	mOutgoingEdges.insert(edge.type(), &edge);
}

void Node::connectEndOf(Edge &edge)
{
	Q_ASSERT_X(!mIncomingEdges.contains(edge.type(), &edge), Q_FUNC_INFO, "Edge end is already connected");
	mIncomingEdges.insert(edge.type(), &edge);
}

void Node::disconnectBeginOf(Edge &edge)
{
	Q_ASSERT_X(mOutgoingEdges.contains(edge.type(), &edge), Q_FUNC_INFO, "Edge begin is not connected");
	mOutgoingEdges.remove(edge.type(), &edge);
}

void Node::disconnectEndOf(Edge &edge)
{
	Q_ASSERT_X(mIncomingEdges.contains(edge.type(), &edge), Q_FUNC_INFO, "Edge end is not connected");
	mIncomingEdges.remove(edge.type(), &edge);
}

void Node::disconnectOutgoing(bool deleteHangingEdges)
{
	while (!mOutgoingEdges.isEmpty()) {
		Edge *edge = *mOutgoingEdges.begin();
		Q_ASSERT(edge && edge->begin() == this);

		edge->disconnectBegin();
		const bool isEdgeHanging = edge->end() == nullptr || edge->end() == this;
		if (deleteHangingEdges && isEdgeHanging) {
			mParent.removeEdge(*edge);
		}
	}
}

void Node::disconnectIncoming(bool deleteHangingEdges)
{
	while (!mIncomingEdges.isEmpty()) {
		Edge *edge = *mIncomingEdges.begin();
		Q_ASSERT(edge && edge->end() == this);

		edge->disconnectEnd();
		const bool isEdgeHanging = edge->begin() == nullptr || edge->begin() == this;
		if (deleteHangingEdges && isEdgeHanging) {
			mParent.removeEdge(*edge);
		}
	}
}

void Node::disconnectAll(bool deleteHangingEdges)
{
	disconnectOutgoing(deleteHangingEdges);
	disconnectIncoming(deleteHangingEdges);
}
