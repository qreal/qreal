#pragma once

#include <QtCore/QPointF>

#include "../../qrkernel/ids.h"
#include "pattern.h"

namespace qReal {

class GroupNode{
public:
	GroupNode(const QString &type, const QString &id, const QPointF &position);

	QString type;
    QString id;
    QPointF position;
};

class GroupEdge{
public:
	GroupEdge(const QString &type, const QString &from, const QString &to);

    QString type;
    QString from;
    QString to;
};

class Pattern{
public:
	Pattern();
//	Pattern(const QString &editor, const QString &diagram, const QString &name, const QString &inNode, const QString &outNode);
	void setName(const QString &name);
    QString getName() const;
	void setEditor(const QString &editor);
	QString editor() const;
	void setDiagram(const QString &diagram);
    QString diagram();
	void addNode(const QString &type, const QString &id, const QPointF &pos);
	void addEdge(const QString &type, const QString &from, const QString &to);
    QList<GroupNode> getNodes() const;
    QList<GroupEdge> getEdges() const;
	void setInNode(const QString &id);
	void setOutNode(const QString &id);
    QString getInNode()const;
    QString getOutNode()const;
	QPointF getSize() const;
	void countSize();

private:
    QString mGroupName;
    QString mEditor;
    QString mDiagram;
    QList<GroupNode> mNodes;
    QList<GroupEdge> mEdges;
    QString mInNode;
	QString mOutNode;
	QPointF mSize;
};
}
