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

typedef int VertexLabel;

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
