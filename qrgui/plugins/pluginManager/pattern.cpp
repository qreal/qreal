#include "pattern.h"

#include <QtCore/QPointF>

#include <qrkernel/ids.h>

#include "editorManager.h"

using namespace qReal;

GroupNode::GroupNode(QString const &type, QString const &id, QPointF const &position, QString const &parent)
	: type(type), id(id), position(position), parent(parent)
{
}

GroupEdge::GroupEdge(QString const &type, QString const &from, QString const &to)
	: type(type), from(from), to(to)
{
}

Pattern::Pattern()
{
}

Pattern::Pattern(QString const &editor, QString const &diagram, QString const &name
		, QString const &inNode, QString const &outNode, QString const &rootNode)
	: mEditor(editor), mDiagram(diagram), mGroupName(name)
	, mInNode(inNode), mOutNode(outNode), mRootNode(rootNode)
{
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

void Pattern::addNode(QString const &type, QString const &id, QPointF const &pos, QString const &parent)
{
	GroupNode newNode(type, id, pos, parent);
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

QString Pattern::rootNode() const
{
	return mRootNode;
}

QString Pattern::rootType() const
{
	for (GroupNode const &node : mNodes) {
		if (node.id == mRootNode) {
			return node.type;
		}
	}

	return QString();
}

void Pattern::setRootNode(QString const &rootId)
{
	mRootNode = rootId;
}

QString Pattern::inNode() const
{
	return mInNode;
}

QString Pattern::outNode() const
{
	return mOutNode;
}

void Pattern::countSize(EditorManager *editorManager)
{
	qreal minY = 0;
	qreal maxY = 0;
	qreal minX = 0;
	qreal maxX = 0;
	for (GroupNode const &node : mNodes) {
		Id const element(mEditor, mDiagram, node.type, "");
		QSize const size = editorManager->iconSize(element);
		if (minY > node.position.y()) {
			minY = node.position.y();
		}
		if (maxY < node.position.y() + size.height()) {
			maxY = node.position.y() + size.height();
		}
		if (minX > node.position.x()) {
			minX = node.position.x();
		}
		if (maxX < node.position.x() + size.width()) {
			maxX = node.position.x() + size.width();
		}
	}

	mSize = QPointF(maxX - minX, maxY - minY);
}

QPointF Pattern::size() const
{
	return mSize;
}
