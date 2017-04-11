/* Copyright 2012-2016 Dmitry Mordvinov
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

#include "clipboard.h"

#include <QtWidgets/QApplication>
#include <QtGui/QClipboard>

#include <qrkernel/definitions.h>
#include <models/models.h>
#include <models/commands/pasteCommand.h>

#include "editor/editorView.h"

using namespace qReal;
using namespace qReal::models;

Clipboard::Clipboard(Controller &controller, const models::Models &models)
	: mController(controller)
	, mModels(models)
{
}

void Clipboard::copy(const IdList &elements)
{
	const QList<NodeInfo> nodesData = this->nodesData(elements);
	const QList<EdgeInfo> edgesData = this->edgesData(elements);

	if (!nodesData.isEmpty() || !edgesData.isEmpty()) {
		pushDataToClipboard(nodesData, edgesData);
	}
}

QList<NodeInfo> Clipboard::nodesData(const IdList &elements)
{
	QList<NodeInfo> nodes;
	for (const Id &item : elements) {
		if (mModels.graphicalModelAssistApi().editorManagerInterface().isNodeOrEdge(item) > 0
				&& !elements.contains(mModels.graphicalModelAssistApi().parent(item)))
		{
			nodes << nodeInfo(item);
		}
	}

	for (const NodeInfo &node : nodes) {
		addChildren(node.id(), nodes);
	}

	return nodes;
}

void Clipboard::addChildren(const Id &node, QList<NodeInfo> &nodes)
{
	const IdList children = mModels.graphicalModelAssistApi().children(node);
	for (const Id &child : children) {
		if (!contains(nodes, child)) {
			nodes << nodeInfo(child);
			addChildren(child, nodes);
		}
	}
}

NodeInfo Clipboard::nodeInfo(const Id &node) const
{
	return NodeInfo(node, mModels.graphicalModelAssistApi().logicalId(node)
					, mModels.logicalModelAssistApi(), mModels.graphicalModelAssistApi());
}

EdgeInfo Clipboard::edgeInfo(const Id &edge) const
{
	return EdgeInfo(edge, mModels.graphicalModelAssistApi().logicalId(edge)
					, mModels.logicalModelAssistApi(), mModels.graphicalModelAssistApi());
}

bool Clipboard::contains(const QList<NodeInfo> &nodes, const Id &id)
{
	for (const NodeInfo &node : nodes) {
		if (node.id() == id) {
			return true;
		}
	}

	return false;
}

QList<EdgeInfo> Clipboard::edgesData(const IdList &elements)
{
	QList<EdgeInfo> edgesData;
	for (const Id &item: elements) {
		if (mModels.graphicalModelAssistApi().editorManagerInterface().isNodeOrEdge(item) < 0) {
			edgesData << edgeInfo(item);
		}
	}

	return edgesData;
}

void Clipboard::pushDataToClipboard(const QList<NodeInfo> &nodesData, const QList<EdgeInfo> &edgesData)
{
	QByteArray data;
	QDataStream stream(&data, QIODevice::WriteOnly);

	stream << nodesData;
	stream << edgesData;

	QMimeData *mimeData = new QMimeData();
	mimeData->setData(DEFAULT_MIME_TYPE, data);

	QClipboard *clipboard = QApplication::clipboard();
	clipboard->setMimeData(mimeData);
}

bool Clipboard::isEmpty() const
{
	const QMimeData *mimeData = QApplication::clipboard()->mimeData();
	return mimeData->data(DEFAULT_MIME_TYPE).isEmpty();
}

void Clipboard::paste(const Id &parent, const QPointF &position, bool isGraphicalCopy)
{
	qReal::commands::PasteCommand *pasteCommand = new qReal::commands::PasteCommand(
			mModels
			, isGraphicalCopy
			, position
			, parent);
	if (!pasteCommand->isEmpty()) {
		mController.execute(pasteCommand);
	}
}
