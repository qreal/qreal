#pragma once

#include <QtCore/QPointF>

#include <qrkernel/ids.h>

#include "qrgui/plugins/pluginManager/pluginsManagerDeclSpec.h"

namespace qReal {

class EditorManager;

class GroupNode
{
public:
	GroupNode(const QString &type, const QString &id, const QPointF &position, const QString &parent);

	QString type;
	QString id;
	QPointF position;
	QString parent;
};

class GroupEdge
{
public:
	GroupEdge(const QString &type, const QString &from, const QString &to);

	QString type;
	QString from;
	QString to;
};

class QRGUI_PLUGINS_MANAGER_EXPORT Pattern
{
public:
	Pattern();
	Pattern(const QString &editor, const QString &diagram, const QString &name
			, const QString &inNode, const QString &outNode, const QString &rootNode);

	void setName(const QString &name);
	QString name() const;

	void setEditor(const QString &editor);
	QString editor() const;

	void setDiagram(const QString &diagram);
	QString diagram() const;

	void addNode(const QString &type, const QString &id, const QPointF &pos, const QString &parent);
	void addEdge(const QString &type, const QString &from, const QString &to);

	QList<GroupNode> nodes() const;
	QList<GroupEdge> edges() const;

	QString inNode() const;
	QString outNode() const;
	void setInNode(const QString &id);
	void setOutNode(const QString &id);

	QString rootNode() const;
	QString rootType() const;
	void setRootNode(const QString &rootId);

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
