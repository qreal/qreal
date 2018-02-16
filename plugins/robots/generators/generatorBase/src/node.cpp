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


Region::Region(RegionType type)
	: mType(type)
{}

RegionType Region::type() const
{
	return mType;
}

QVector<VertexLabel> Region::nodes() const
{
	return QVector<VertexLabel>();
}

BlockRegion::BlockRegion(QVector<VertexLabel> &nodes)
	: Region(Block)
	, mNodes(nodes)
{}

void BlockRegion::addVertex(VertexLabel vertexLabel)
{
	mNodes.push_back(vertexLabel);
}

QVector<VertexLabel> BlockRegion::nodes() const
{
	return mNodes;
}

IfThenElseRegion::IfThenElseRegion()
	: Region(IfThenElse)
{}

QVector<VertexLabel> IfThenElseRegion::nodes() const
{
	return {mCondition, mThen, mElse};
}

void IfThenElseRegion::addCondition(VertexLabel conditionLabel)
{
	mCondition = conditionLabel;
}

void IfThenElseRegion::addThen(VertexLabel thenLabel)
{
	mThen = thenLabel;
}

void IfThenElseRegion::addElse(VertexLabel elseLabel)
{
	mElse = elseLabel;
}

VertexLabel IfThenElseRegion::getCondition() const
{
	return mCondition;
}

VertexLabel IfThenElseRegion::getThen() const
{
	return mThen;
}

VertexLabel IfThenElseRegion::getElse() const
{
	return mElse;
}

IfThenRegion::IfThenRegion()
	: Region(IfThen)
{}

void IfThenRegion::addCondition(VertexLabel conditionLabel)
{
	mCondition = conditionLabel;
}

void IfThenRegion::addThen(VertexLabel thenLabel)
{
	mThen = thenLabel;
}

VertexLabel IfThenRegion::getCondition() const
{
	return mCondition;
}

VertexLabel IfThenRegion::getThen() const
{
	return mThen;
}

NilRegion::NilRegion()
	: Region(nil)
{}

SelfLoopRegion::SelfLoopRegion(VertexLabel body)
	: Region(SelfLoop)
	, mBody(body)
{}

void SelfLoopRegion::addBody(VertexLabel body)
{
	mBody = body;
}

VertexLabel SelfLoopRegion::body() const
{
	return mBody;
}

WhileLoopRegion::WhileLoopRegion(VertexLabel condition, VertexLabel body)
	: Region(WhileLoop)
	, mCondition(condition)
	, mBody(body)
{}

void WhileLoopRegion::addBody(VertexLabel body)
{
	mBody = body;
}

void WhileLoopRegion::addCondition(VertexLabel condition)
{
	mCondition = condition;
}

VertexLabel WhileLoopRegion::body() const
{
	return mBody;
}

VertexLabel WhileLoopRegion::condition() const
{
	return mCondition;
}
