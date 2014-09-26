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

	void cut();
	void copy();
	void paste(bool isGraphicalCopy);

private:
	void pushDataToClipboard(QList<NodeData> const &nodesData, QList<EdgeData> const &edgesData) const;
	QList<NodeElement *> getNodesForCopying() const;
	QList<EdgeElement *> getEdgesForCopying(QList<NodeElement *> const &nodes) const;
	QList<NodeData> getNodesData(QList<NodeElement *> const &nodes) const;
	QList<EdgeData> getEdgesData(QList<EdgeElement *> const &edges) const;

	void addChildren(NodeElement *node, QList<NodeElement *> &nodes) const;
	QSet<EdgeElement *> edgesInContainer(QList<NodeElement *> const &nodes) const;
	IdList toIdList(QList<NodeElement *> const &nodes) const;

	EditorViewScene *mScene;
	EditorViewMViface *mMVIface;
	Controller *mController;
};

}
