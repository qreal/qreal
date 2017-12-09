/* Copyright 2017 QReal Research Group
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

#include "generatorBase/node.h"

using namespace graphUtils;

const int MAXID = 1e7;

Node::Node()
	: mId(MAXID)
	, mRegionType(nil)
	, mParent(-1)
{
}

Node::Node(int id, RegionType type)
	: mId(id)
	, mRegionType(type)
	, mParent(-1)
{
}

void Node::appendChild(VertexLabel child)
{
	mChildren.append(child);
}

void Node::appendChildren(const QVector<VertexLabel> &children)
{
	for (VertexLabel child : children) {
		mChildren.append(child);
	}
}

VertexLabel Node::structOf() const
{
	return mParent;
}

void Node::setParent(VertexLabel parent)
{
	mParent = parent;
}

RegionType Node::structType() const
{
	return mRegionType;
}

QVector<VertexLabel> Node::structNodes() const
{
	return mChildren;
}

bool Node::operator <(const Node &other) const
{
	return mId < other.id();
}

int Node::id() const
{
	return mId;
}

