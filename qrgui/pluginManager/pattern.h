#pragma once

#include <QtCore/QPointF>

#include "../../qrkernel/ids.h"
#include "pattern.h"

namespace qReal {
class EditorManager;
class GroupNode{
public:
	GroupNode(QString const &typeArg, QString const &idArg, QPointF const &positionArg);

	QString type;
	QString id;
	QPointF position;
};

class GroupEdge{
public:
	GroupEdge(QString const &typeArg, QString const &fromArg, QString const &toArg);

	QString type;
	QString from;
	QString to;
};

class Pattern{
public:
	Pattern();
	Pattern(QString const &editor, QString const &diagram, QString const &name, QString const &inNode, QString const &outNode);
	void setName(QString const &name);
	QString name() const;
	void setEditor(QString const &editor);
	QString editor() const;
	void setDiagram(QString const &diagram);
	QString diagram();
	void addNode(QString const &type, QString const &id, QPointF const &pos);
	void addEdge(QString const &type, QString const &from, QString const &to);
	QList<GroupNode> nodes() const;
	QList<GroupEdge> edges() const;
	void setInNode(QString const &id);
	void setOutNode(QString const &id);
	QString inNode()const;
	QString outNode()const;
	QPointF size() const;
	void countSize(EditorManager *editorManager);

private:
	QString mEditor;
	QString mDiagram;
	QString mGroupName;
	QList<GroupNode> mNodes;
	QList<GroupEdge> mEdges;
	QString mInNode;
	QString mOutNode;
	QPointF mSize;
};
}
