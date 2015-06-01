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

#include "pattern.h"

#include <QtCore/QPointF>

#include <qrkernel/ids.h>

#include "editorManager.h"

using namespace qReal;

GroupNode::GroupNode(const QString &type, const QString &id, const QPointF &position, const QString &parent)
	: type(type), id(id), position(position), parent(parent)
{
}

GroupEdge::GroupEdge(const QString &type, const QString &from, const QString &to)
	: type(type), from(from), to(to)
{
}

Pattern::Pattern()
{
}

Pattern::Pattern(const QString &editor, const QString &diagram, const QString &name
		, const QString &inNode, const QString &outNode, const QString &rootNode)
	: mEditor(editor), mDiagram(diagram), mGroupName(name)
	, mInNode(inNode), mOutNode(outNode), mRootNode(rootNode)
{
}

void Pattern::setEditor(const QString &editor)
{
	mEditor = editor;
}

QString Pattern::editor() const
{
	return mEditor;
}

void Pattern::setDiagram(const QString &diagram)
{
	mDiagram = diagram;
}

QString Pattern::diagram() const
{
	return mDiagram;
}

void Pattern::setName(const QString &name)
{
	mGroupName = name;
}

QString Pattern::name() const
{
	return mGroupName;
}

void Pattern::addNode(const QString &type, const QString &id, const QPointF &pos, const QString &parent)
{
	GroupNode newNode(type, id, pos, parent);
	mNodes.append(newNode);
}

void Pattern::addEdge(const QString &type, const QString &from, const QString &to)
{
	GroupEdge newEdge(type, from, to);
	mEdges.append(newEdge);
}

QList<GroupNode> Pattern::nodes() const
{
	return mNodes;
}

QList<GroupEdge> Pattern::edges() const
{
	return mEdges;
}

void Pattern::setInNode(const QString &id)
{
	mInNode = id;
}

void Pattern::setOutNode(const QString &id)
{
	mOutNode = id;
}

QString Pattern::rootNode() const
{
	return mRootNode;
}

QString Pattern::rootType() const
{
	for (const GroupNode &node : mNodes) {
		if (node.id == mRootNode) {
			return node.type;
		}
	}

	return QString();
}

void Pattern::setRootNode(const QString &rootId)
{
	mRootNode = rootId;
}

QString Pattern::inNode() const
{
	return mInNode;
}

QString Pattern::outNode() const
{
	return mOutNode;
}

void Pattern::countSize(EditorManager *editorManager)
{
	qreal minY = 0;
	qreal maxY = 0;
	qreal minX = 0;
	qreal maxX = 0;
	for (const GroupNode &node : mNodes) {
		const Id element(mEditor, mDiagram, node.type, "");
		const QSize size = editorManager->iconSize(element);
		if (minY > node.position.y()) {
			minY = node.position.y();
		}
		if (maxY < node.position.y() + size.height()) {
			maxY = node.position.y() + size.height();
		}
		if (minX > node.position.x()) {
			minX = node.position.x();
		}
		if (maxX < node.position.x() + size.width()) {
			maxX = node.position.x() + size.width();
		}
	}

	mSize = QPointF(maxX - minX, maxY - minY);
}

QPointF Pattern::size() const
{
	return mSize;
}
