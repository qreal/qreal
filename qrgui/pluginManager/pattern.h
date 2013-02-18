#ifndef PATTERN_H
#define PATTERN_H


#include "../../qrkernel/ids.h"
#include "pattern.h"
#include <QtCore/QPointF>

namespace qReal {
class groupNode{
public:
    QString type;
    QString id;
    QPointF position;
};

class groupEdge{
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
    QList<groupNode> getNodes() const;
    QList<groupEdge> getEdges() const;
    void setInNode(QString id);
    void setOutNode(QString id);
    QString getInNode()const;
    QString getOutNode()const;

private:
    QString mGroupName;
    QString mEditor;
    QString mDiagram;
    QList<groupNode> mNodes;
    QList<groupEdge> mEdges;
    QString mInNode;
    QString mOutNode;

};

}

#endif // PATTERN_H
