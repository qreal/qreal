#pragma once

#include <QtCore/QObject>

#include "qrgui/editor/nodeElement.h"
#include "qrgui/editor/edgeElement.h"
#include "qrgui/editor/serializationData.h"

namespace qReal {

class EditorViewScene;

class ClipboardHandler
{
public:
	ClipboardHandler(EditorViewScene &scene, Controller &controller);

	void cut();
	void copy();
	void paste(bool isGraphicalCopy);

private:
	void pushDataToClipboard(QList<NodeData> const &nodesData, QList<EdgeData> const &edgesData);
	QList<NodeElement *> getNodesForCopying();
	QList<NodeData> getNodesData(QList<NodeElement *> const &nodes);
	QList<EdgeData> getEdgesData();
	void addChildren(NodeElement *node, QList<NodeElement *> &nodes);

	EditorViewScene &mScene;
	Controller &mController;
};

}
