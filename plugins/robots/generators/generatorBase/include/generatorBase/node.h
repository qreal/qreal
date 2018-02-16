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
#pragma once

#include <QVector>

namespace graphUtils
{

typedef int VertexLabel;

enum RegionType {
	Block = 0
	, IfThen
	, IfThenElse
	, Case
	, Proper
	, SelfLoop
	, WhileLoop
	, NaturalLoop
	, Improper
	, nil
	, simpleNode
};


class Region {
public:
	Region(RegionType type);
	virtual ~Region() {}
	RegionType type() const;
	virtual QVector<VertexLabel> nodes() const;

protected:
	RegionType mType;
};

class BlockRegion : public Region {
public:
	BlockRegion(QVector<VertexLabel> &nodes);

	void addVertex(VertexLabel vertexLabel);
	QVector<VertexLabel> nodes() const;

private:
	QVector<VertexLabel> mNodes;
};


class IfThenElseRegion : public Region {
public:
	IfThenElseRegion();

	QVector<VertexLabel> nodes() const;

	void addCondition(VertexLabel conditionLabel);
	void addThen(VertexLabel thenLabel);
	void addElse(VertexLabel elseLabel);
	VertexLabel getCondition() const;
	VertexLabel getThen() const;
	VertexLabel getElse() const;

private:
	VertexLabel mCondition;
	VertexLabel mThen;
	VertexLabel mElse;
};

class IfThenRegion : public Region {
public:
	IfThenRegion();

	void addCondition(VertexLabel conditionLabel);
	void addThen(VertexLabel thenLabel);
	VertexLabel getCondition() const;
	VertexLabel getThen() const;

private:
	VertexLabel mCondition;
	VertexLabel mThen;
};

class SelfLoopRegion : public Region {
public:
	SelfLoopRegion(VertexLabel body);

	void addBody(VertexLabel body);
	VertexLabel body() const;

private:
	VertexLabel mBody;
};

class WhileLoopRegion : public Region {
public:
	WhileLoopRegion(VertexLabel condition, VertexLabel body);

	void addCondition(VertexLabel condition);
	void addBody(VertexLabel body);
	VertexLabel body() const;
	VertexLabel condition() const;

private:
	VertexLabel mCondition;
	VertexLabel mBody;
};


class NilRegion : public Region {
public:
	NilRegion();
};


class Node {
public:
	Node();
	Node(int id, RegionType type);

	void appendChild(VertexLabel child);
	void appendChildren(const QVector<VertexLabel> &children);

	VertexLabel structOf() const;
	void setParent(VertexLabel parent);
	RegionType structType() const;
	QVector<VertexLabel> structNodes() const;

	bool operator <(const Node& other) const;

	int id() const;

protected:
	int mId;
	RegionType mRegionType;
	VertexLabel mParent;
	QVector<VertexLabel> mChildren;
};

}
