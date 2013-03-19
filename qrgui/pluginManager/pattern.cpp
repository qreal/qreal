#include <QtCore/QPointF>

#include "../../qrkernel/ids.h"
#include "pattern.h"

using namespace qReal;
GroupNode::GroupNode(const QString &type, const QString &id, const QPointF &position){
	this->type = type;
	this->id = id;
	this->position = position;
}

GroupEdge::GroupEdge(const QString &type, const QString &from, const QString &to){
	this->type = type;
	this->from = from;
	this->to = to;
}

Pattern::Pattern(){}
/*Pattern::Pattern(const QString &editor, const QString &diagram,
				 const QString &name, const QString &inNode, const QString &outNode)
{
	mEditor = editor;
	mDiagram = diagram;
	mGroupName = name;
	mInNode = inNode;
	mOutNode = outNode;
}//*/

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

QString Pattern::diagram()
{
    return mDiagram;
}

void Pattern::setName(const QString &name)
{
    mGroupName = name;
}

QString Pattern::getName() const
{
    return mGroupName;
}

void Pattern::addNode(const QString &type, const QString &id, const QPointF &pos)
{
	GroupNode newNode = GroupNode(type, id, pos);
    mNodes.append(newNode);
}

void Pattern::addEdge(const QString &type, const QString &from, const QString &to)
{
	GroupEdge newEdge = GroupEdge(type, from, to);
    mEdges.append(newEdge);
}

QList<GroupNode> Pattern::getNodes() const
{
    return mNodes;
}

QList<GroupEdge> Pattern::getEdges()const
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

QString Pattern::getInNode()const
{
    return mInNode;
}

QString Pattern::getOutNode()const
{
    return mOutNode;
}

void Pattern::countSize(){
	qreal minY = 0;
	qreal maxY = 0;
	qreal minX = 0;
	qreal maxX = 0;
	qreal sizeX = 80;
	qreal sizeY = 60;
	foreach (const GroupNode &node, this->mNodes)
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
	mSize = QPointF(maxX-minX + sizeX, maxY-minY + sizeY);
}

QPointF Pattern::getSize() const{
	return mSize;
}
