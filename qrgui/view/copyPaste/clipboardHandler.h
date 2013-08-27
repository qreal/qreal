#pragma once

#include <QtCore/QObject>

#include "umllib/nodeElement.h"
#include "umllib/edgeElement.h"
#include "umllib/serializationData.h"

namespace qReal
{

class EditorViewScene;
class EditorViewMViface;

class ClipboardHandler
{
public:
	ClipboardHandler(EditorViewScene *scene);

	void setMVIface(EditorViewMViface * const mvIface);
	void setController(Controller * const controller);

	void copy();
	void paste(bool isGraphicalCopy);

private:
	void pushDataToClipboard(QList<NodeData> const &nodesData, QList<EdgeData> const &edgesData);
	QList<NodeElement *> getNodesForCopying();
	QList<NodeData> getNodesData(QList<NodeElement *> const &nodes);
	QList<EdgeData> getEdgesData();
	void addChildren(NodeElement *node, QList<NodeElement *> &nodes);

	EditorViewScene *mScene;
	EditorViewMViface *mMVIface;
	Controller *mController;
};

}
