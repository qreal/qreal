#include <QtCore/QPointF>

#include "../../qrkernel/ids.h"
#include "pattern.h"

using namespace qReal;
Pattern::Pattern()
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

void Pattern::addNode(QString type, QString id, QPointF pos)
{
    GroupNode newNode;
    newNode.type = type;
    newNode.id = id;
    newNode.position = pos;
    mNodes.append(newNode);
}

void Pattern::addEdge(QString type, QString from, QString to)
{
    GroupEdge newEdge;
    newEdge.type = type;
    newEdge.from = from;
    newEdge.to = to;
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

void Pattern::setInNode(QString id)
{
    mInNode = id;
}

void Pattern::setOutNode(QString id)
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
