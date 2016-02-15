/* Copyright 2013-2016 QReal Research Group, Dmitry Mordvinov
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

#include "models/commands/pasteCommand.h"

#include <QtWidgets/QApplication>
#include <QtGui/QClipboard>

#include "models/models.h"
#include "models/nodeInfo.h"
#include "models/edgeInfo.h"

using namespace qReal::commands;

PasteCommand::PasteCommand(const models::Models &models
		, bool isGraphicalCopy
		, const QPointF &mousePosition
		, const Id &rootGraphicalId)
	: CreateElementsCommand(models, {})
	, mIsGraphicalCopy(isGraphicalCopy)
	, mMousePosition(mousePosition)
	, mRootDiagramGraphicalId(rootGraphicalId)
	, mIsEmpty(false)
{
	QList<NodeInfo> nodesData;
	QList<EdgeInfo> edgesData;
	pullDataFromClipboard(nodesData, edgesData);

	if (nodesData.isEmpty() && edgesData.isEmpty()) {
		mIsEmpty = true;
		return;
	}

	const QPointF originalPos = nodesData.isEmpty() ? edgesData[0].position() : nodesData[0].position();
	const QPointF offset = mMousePosition - originalPos;

	const QHash<Id, Id> copiedIds = prepareNodes(models.graphicalModelAssistApi(), nodesData, offset);

	QList<ElementInfo> allElements;
	for (const ElementInfo &node : nodesData) {
		allElements << node;
	}
	for (EdgeInfo &edge : edgesData) {
		prepareEdge(edge, offset, copiedIds);
		const ElementInfo elemInfo = edge.convertToSimpleInfo();
		allElements << elemInfo;
	}
	setElements(allElements);
}

bool PasteCommand::isEmpty() const
{
	return mIsEmpty;
}

QHash<qReal::Id, qReal::Id> PasteCommand::prepareNodes(models::GraphicalModelAssistApi &graphicalApi
		, QList<NodeInfo> &nodesData, const QPointF &offset)
{
	QHash<Id, Id> copiedIds;
	// Stage I: First generating new ids. All ids must be generated before next stage because some elements need new
	// ids of their parents.
	for (NodeInfo &node : nodesData) {
		if (!mIsGraphicalCopy) {
			copiedIds[graphicalApi.logicalId(node.id())] = node.newLogicalId();
		}

		// Do not move node.id into brackets (evaluation order).
		const Id oldId = node.id();
		copiedIds[oldId] = node.newId();
	}

	// Stage II: Setting new elements positions and graphical parents. Parents may be copied together with child,
	// then it will be child of new parent element, or not copied, then parent will be root diagram.
	for (NodeInfo &node : nodesData) {
		node.setPos(mIsGraphicalCopy ? newGraphicalPos(node, copiedIds, offset) : newPos(node, copiedIds, offset));
		node.setGraphicalParent(newGraphicalParent(node, copiedIds));
	}

	return copiedIds;
}

void PasteCommand::prepareEdge(EdgeInfo &edgeData, const QPointF &offset, const QHash<Id, Id> &copiedIds)
{
	edgeData.newId();
	if (!mIsGraphicalCopy) {
		edgeData.newLogicalId();
	}

	edgeData.setSrcId(copiedIds.contains(edgeData.srcId()) ? copiedIds[edgeData.srcId()] : Id::rootId());
	edgeData.setDstId(copiedIds.contains(edgeData.dstId()) ? copiedIds[edgeData.dstId()] : Id::rootId());
	edgeData.setPos(edgeData.position() + offset);
	edgeData.setGraphicalParent(newGraphicalParent(edgeData, copiedIds));
}

void PasteCommand::pullDataFromClipboard(QList<NodeInfo> &nodesData, QList<EdgeInfo> &edgesData) const
{
	const QClipboard *clipboard = QApplication::clipboard();
	const QMimeData *mimeData = clipboard->mimeData();

	QByteArray data = mimeData->data("application/x-real-uml-model-data");
	QDataStream stream(&data, QIODevice::ReadOnly);

	stream >> nodesData;
	stream >> edgesData;
}

QPointF PasteCommand::newPos(const NodeInfo &nodeData, const QHash<Id, Id> &copiedIds, const QPointF &offset) const
{
	return nodeData.position() + (copiedIds.contains(nodeData.parent()) ?
			mGraphicalApi.position(copiedIds[nodeData.parent()]) : offset);
}

QPointF PasteCommand::newGraphicalPos(const NodeInfo &nodeData
		, const QHash<Id, Id> &copiedIds, const QPointF &offset) const
{
	return nodeData.position() + (copiedIds.contains(nodeData.parent()) ? QPointF() : offset);
}

qReal::Id PasteCommand::newGraphicalParent(const ElementInfo &data, const QHash<Id, Id> &copiedIds) const
{
	return copiedIds.contains(data.graphicalParent())
			? copiedIds[data.graphicalParent()]
			: mRootDiagramGraphicalId;
}

QPointF PasteCommand::vectorFromContainer(const NodeInfo &nodeData) const
{
	return (nodeData.parent() == Id::rootId()) ? QPointF() : nodeData.position();
}
