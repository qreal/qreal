/* Copyright 2007-2015 QReal Research Group
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

#include "qrtext/core/ast/node.h"

using namespace qrtext::core;
using namespace qrtext::core::ast;

const Connection Node::noConnection = Connection();
int Node::mNodesCount = 0;

Node::Node()
{
	++mNodesCount;
}

Node::~Node()
{
	--mNodesCount;
}

const Connection &Node::start() const
{
	if (mRanges.isEmpty()) {
		return noConnection;
	} else {
		return mRanges.first().start();
	}
}

const Connection &Node::end() const
{
	if (mRanges.isEmpty()) {
		return noConnection;
	} else {
		return mRanges.last().end();
	}
}

QList<Range> const &Node::ranges() const
{
	return mRanges;
}

void Node::connect(QSharedPointer<Node> const &node)
{
	if (node) {
		connect(node->ranges());
	}
}

void Node::connect(QList<Range> const &ranges)
{
	mRanges << ranges;
	qSort(mRanges.begin(), mRanges.end(), [](Range a, Range b) { return a.start() < b.start(); });
}

void Node::acceptRecursively(AstVisitorInterface &visitor
		, const QSharedPointer<Node> &pointer
		, const QSharedPointer<Node> &parent)
{
	for (const auto &node : children()) {
		if (node.data()) {
			node->acceptRecursively(visitor, node, pointer);
		}
	}

	accept(visitor, pointer, parent);
}

void Node::accept(AstVisitorInterface &visitor, const QSharedPointer<Node> &pointer, const QSharedPointer<Node> &parent)
{
	visitor.visit(qSharedPointerCast<ast::Node>(pointer), parent);
}
