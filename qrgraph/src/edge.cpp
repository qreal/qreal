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

#include "qrgraph/edge.h"

#include "qrgraph/multigraph.h"
#include "qrgraph/node.h"

using namespace qrgraph;

Edge::Edge(Multigraph &parent, unsigned int type)
	: mParent(parent)
	, mType(type)
	, mBegin(nullptr)
	, mEnd(nullptr)
{
}

Edge::~Edge()
{
	disconnect();
}

Multigraph &Edge::graph() const
{
	return mParent;
}

unsigned int Edge::type() const
{
	return mType;
}

Node *Edge::begin() const
{
	return mBegin;
}

Node *Edge::end() const
{
	return mEnd;
}

void Edge::connectBegin(Node &node)
{
	disconnectBegin();
	node.connectBeginOf(*this);
	mBegin = &node;
}

void Edge::connectEnd(Node &node)
{
	node.connectEndOf(*this);
	mEnd = &node;
}

void Edge::connect(Node &begin, Node &end)
{
	connectBegin(begin);
	connectEnd(end);
}

void Edge::disconnectBegin()
{
	if (mBegin) {
		mBegin->disconnectBeginOf(*this);
		mBegin = nullptr;
	}
}

void Edge::disconnectEnd()
{
	if (mEnd) {
		mEnd->disconnectEndOf(*this);
		mEnd = nullptr;
	}
}

void Edge::disconnect()
{
	disconnectBegin();
	disconnectEnd();
}
