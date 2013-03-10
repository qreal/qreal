#ifndef PATTERN_H
#define PATTERN_H

#include "../../qrkernel/ids.h"

#include "pattern.h"
#include <QtCore/QPointF>

namespace qReal {
class GroupNode{
public:
    QString type;
    QString id;
    QPointF position;
};

class GroupEdge{
public:
    QString type;
    QString from;
    QString to;
};

class Pattern
{
public:
    Pattern();
    void setName(QString name);
    QString getName() const;

    void setEditor(QString editor);
    QString editor();
    void setDiagram(QString diagram);
    QString diagram();
    void addNode(QString type, QString id, QPointF pos);
    void addEdge(QString type, QString from, QString to);
    QList<GroupNode> getNodes() const;
    QList<GroupEdge> getEdges() const;
    void setInNode(QString id);
    void setOutNode(QString id);
    QString getInNode()const;
    QString getOutNode()const;

private:
    QString mGroupName;
    QString mEditor;
    QString mDiagram;
    QList<GroupNode> mNodes;
    QList<GroupEdge> mEdges;
    QString mInNode;
    QString mOutNode;

};

}

#endif // PATTERN_H
