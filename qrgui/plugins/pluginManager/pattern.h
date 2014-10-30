#pragma once

#include <QtCore/QPointF>

#include <qrkernel/ids.h>

namespace qReal {

class EditorManager;

class GroupNode
{
public:
	GroupNode(QString const &type, QString const &id, QPointF const &position, QString const &parent);

	QString type;
	QString id;
	QPointF position;
	QString parent;
};

class GroupEdge
{
public:
	GroupEdge(QString const &type, QString const &from, QString const &to);

	QString type;
	QString from;
	QString to;
};

class Pattern
{
public:
	Pattern();
	Pattern(QString const &editor, QString const &diagram, QString const &name
			, QString const &inNode, QString const &outNode, QString const &rootNode);

	void setName(QString const &name);
	QString name() const;

	void setEditor(QString const &editor);
	QString editor() const;

	void setDiagram(QString const &diagram);
	QString diagram();

	void addNode(QString const &type, QString const &id, QPointF const &pos, QString const &parent);
	void addEdge(QString const &type, QString const &from, QString const &to);

	QList<GroupNode> nodes() const;
	QList<GroupEdge> edges() const;

	QString inNode()const;
	QString outNode()const;
	void setInNode(QString const &id);
	void setOutNode(QString const &id);

	QString rootNode() const;
	QString rootType() const;
	void setRootNode(QString const &rootId);

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
	QString mRootNode;
	QPointF mSize;
};

inline bool operator==(GroupNode const &node1, GroupNode const &node2)
{
	return node1.type == node2.type
			&& node1.id == node2.id
			&& node1.parent == node2.parent
			&& node1.position == node2.position;
}

}
