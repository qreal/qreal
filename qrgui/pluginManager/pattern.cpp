#include <QtCore/QPointF>

#include "../../qrkernel/ids.h"
#include "pattern.h"

using namespace qReal;
GroupNode::GroupNode(QString const &type, QString const &id, QPointF const &position)
{
	this->type = type;
	this->id = id;
	this->position = position;
}

GroupEdge::GroupEdge(QString const &type, QString const &from, QString const &to)
{
	this->type = type;
	this->from = from;
	this->to = to;
}

Pattern::Pattern()
{
}

Pattern::Pattern(QString const &editor, QString const &diagram,
				 QString const &name, QString const &inNode, QString const &outNode)
{
	mEditor = editor;
	mDiagram = diagram;
	mGroupName = name;
	mInNode = inNode;
	mOutNode = outNode;
}

void Pattern::setEditor(QString const &editor)
{
	mEditor = editor;
}

QString Pattern::editor() const
{
	return mEditor;
}

void Pattern::setDiagram(QString const &diagram)
{
	mDiagram = diagram;
}

QString Pattern::diagram()
{
	return mDiagram;
}

void Pattern::setName(QString const &name)
{
	mGroupName = name;
}

QString Pattern::name() const
{
	return mGroupName;
}

void Pattern::addNode(QString const &type, QString const &id, QPointF const &pos)
{
	GroupNode newNode(type, id, pos);
	mNodes.append(newNode);
}

void Pattern::addEdge(QString const &type, QString const &from, QString const &to)
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

void Pattern::setInNode(QString const &id)
{
	mInNode = id;
}

void Pattern::setOutNode(QString const &id)
{
	mOutNode = id;
}

QString Pattern::inNode() const
{
	return mInNode;
}

QString Pattern::outNode() const
{
	return mOutNode;
}

void Pattern::countSize()
{
	qreal minY = 0;
	qreal maxY = 0;
	qreal minX = 0;
	qreal maxX = 0;
	foreach (GroupNode const &node, mNodes)
	{
		if (minY > node.position.y()){
			minY = node.position.y();
		}
		if (maxY < node.position.y()){
			maxY = node.position.y();
		}
		if (minX > node.position.x()){
			minX = node.position.x();
		}
		if (maxX < node.position.x()){
			maxX = node.position.x();
		}
	}
	mSize = QPointF(maxX-minX, maxY-minY);
}

QPointF Pattern::size() const
{
	return mSize;
}
