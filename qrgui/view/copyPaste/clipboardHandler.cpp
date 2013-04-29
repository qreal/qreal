#include <QtWidgets/QApplication>
#include <QtGui/QClipboard>

#include "clipboardHandler.h"
#include "pasteGroupCommand.h"
#include "../editorViewScene.h"

using namespace qReal;

ClipboardHandler::ClipboardHandler(EditorViewScene *scene)
	: mScene(scene), mMVIface(NULL)
{
}

void ClipboardHandler::setMVIface(EditorViewMViface *mvIface)
{
	mMVIface = mvIface;
}

void ClipboardHandler::setController(Controller * const controller)
{
	mController = controller;
}

void ClipboardHandler::copy()
{
	QList<NodeElement *> nodes = getNodesForCopying();

	QList<NodeData> nodesData = getNodesData(nodes);
	QList<EdgeData> edgesData = getEdgesData(nodes);

	pushDataToClipboard(nodesData, edgesData);
}

QList<NodeData> ClipboardHandler::getNodesData(QList<NodeElement *> const &nodes)
{
	QList<NodeData> nodesData;
	foreach (NodeElement* node, nodes) {
		nodesData << node->data();
	}
	return nodesData;
}

QList<NodeElement *> ClipboardHandler::getNodesForCopying()
{
	QList<NodeElement *> nodes;
	foreach (QGraphicsItem *item, mScene->selectedItems()) {
		NodeElement *node = dynamic_cast<NodeElement *>(item);
		if (node && !mScene->selectedItems().contains(node->parentItem())) {
			nodes << node;
		}
	}
	foreach (NodeElement *node, nodes) {
		addChildren(node, nodes);
	}

	return nodes;
}

void ClipboardHandler::addChildren(NodeElement *node, QList<NodeElement *> &nodes)
{
	foreach (QGraphicsItem *item, node->childItems()) {
		NodeElement *child = dynamic_cast<NodeElement *>(item);
		if (child && !nodes.contains(child)) {
			nodes << child;
			addChildren(child, nodes);
		}
	}
}

QList<EdgeData> ClipboardHandler::getEdgesData(QList<NodeElement *> const &nodes)
{
	QList<EdgeData> edgesData;
	foreach (NodeElement* node, nodes) {
		foreach (EdgeElement *edge, node->getEdges()) {
			EdgeData& data = edge->data();
			if (nodes.contains(edge->src()) && nodes.contains(edge->dst())
					&& !edgesData.contains(data)) {
				edgesData << data;
			}
		}
	}
	return edgesData;
}

void ClipboardHandler::pushDataToClipboard(QList<NodeData> const &nodesData, QList<EdgeData> const &edgesData)
{
	QByteArray data;
	QDataStream stream(&data, QIODevice::WriteOnly);

	stream << nodesData;
	stream << edgesData;

	QMimeData *mimeData = new QMimeData();
	mimeData->setData("application/x-real-uml-model-data", data);

	QClipboard *clipboard = QApplication::clipboard();
	clipboard->setMimeData(mimeData);
}

void ClipboardHandler::paste(bool isGraphicalCopy)
{
	mController->execute(new commands::PasteGroupCommand(mScene, mMVIface, isGraphicalCopy));
}
