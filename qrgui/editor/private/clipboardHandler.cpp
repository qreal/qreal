/* Copyright 2007-2016 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "clipboardHandler.h"

#include <QtWidgets/QApplication>
#include <QtGui/QClipboard>

#include <models/models.h>
#include <models/commands/pasteCommand.h>

#include "editor/editorView.h"

using namespace qReal;
using namespace qReal::gui::editor;

ClipboardHandler::ClipboardHandler(EditorViewScene &scene, Controller &controller)
	: mScene(scene)
	, mController(controller)
{
}

void ClipboardHandler::cut()
{
	copy();
	mScene.deleteSelectedItems();
}

void ClipboardHandler::copy()
{
	QList<NodeElement *> nodes = getNodesForCopying();

	QList<NodeInfo> nodesData = getNodesData(nodes);
	QList<EdgeInfo> edgesData = getEdgesData();

	if (!nodesData.isEmpty() || !edgesData.isEmpty()) {
		pushDataToClipboard(nodesData, edgesData);
	}
}

QList<NodeInfo> ClipboardHandler::getNodesData(QList<NodeElement *> const &nodes)
{
	QList<NodeInfo> nodesData;
	for (NodeElement * const node : nodes) {
		nodesData << node->data();
	}

	return nodesData;
}

QList<NodeElement *> ClipboardHandler::getNodesForCopying()
{
	QList<NodeElement *> nodes;
	for (QGraphicsItem * const item : mScene.selectedItems()) {
		NodeElement *node = dynamic_cast<NodeElement *>(item);
		if (node && !mScene.selectedItems().contains(node->parentItem())) {
			nodes << node;
		}
	}

	for (NodeElement * const node : nodes) {
		addChildren(node, nodes);
	}

	return nodes;
}

void ClipboardHandler::addChildren(NodeElement *node, QList<NodeElement *> &nodes)
{
	for (QGraphicsItem *item : node->childItems()) {
		NodeElement *child = dynamic_cast<NodeElement *>(item);
		if (child && !nodes.contains(child)) {
			nodes << child;
			addChildren(child, nodes);
		}
	}
}

QPointF ClipboardHandler::currentMousePosInSceneCoordinates() const
{
	const EditorView *editor = nullptr;
	for (const QGraphicsView * const view : mScene.views()) {
		if ((editor = dynamic_cast<const EditorView *>(view))) {
			break;
		}
	}

	if (!editor) {
		return QPointF();
	}

	return editor->mapToScene(editor->mapFromGlobal(QCursor::pos()));
}

QList<EdgeInfo> ClipboardHandler::getEdgesData()
{
	QList<EdgeInfo> edgesData;
	for (QGraphicsItem *item : mScene.selectedItems()) {
		EdgeElement *edge = dynamic_cast<EdgeElement *>(item);
		if (edge) {
			edgesData << edge->data();
		}
	}

	return edgesData;
}

void ClipboardHandler::pushDataToClipboard(const QList<NodeInfo> &nodesData, const QList<EdgeInfo> &edgesData)
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
	qReal::commands::PasteCommand *pasteCommand = new qReal::commands::PasteCommand(
			mScene.models()
			, isGraphicalCopy
			, currentMousePosInSceneCoordinates()
			, mScene.rootItemId());
	if (!pasteCommand->isEmpty()) {
		mController.execute(pasteCommand);
	}
}
