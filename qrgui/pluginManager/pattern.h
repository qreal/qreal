#pragma once

#include <QtCore/QPointF>

#include "../../qrkernel/ids.h"
#include "pattern.h"

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
	void setName(const QString &name);
    QString getName() const;

	void setEditor(const QString &editor);
	QString editor() const;
	void setDiagram(const QString &diagram);
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
