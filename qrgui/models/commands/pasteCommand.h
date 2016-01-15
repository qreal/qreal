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

#pragma once

#include "qrgui/models/commands/createElementsCommand.h"

namespace qReal {

class NodeInfo;
class EdgeInfo;

namespace commands {

/// Implements pasting elements from clipboard into the scene.
class QRGUI_MODELS_EXPORT PasteCommand : public qReal::commands::CreateElementsCommand
{
public:
	/// @param isGraphicalCopy If true only graphical part will be pasted, logical part will not be created.
	/// @param mousePosition Mouse cursor position in scene coordinates in moment of paste action triggering.
	/// @param rootGraphicalId Graphical id of root element of the scene elements were pasted into.
	PasteCommand(const models::Models &models
			, bool isGraphicalCopy
			, const QPointF &mousePosition
			, const Id &rootGraphicalId);

	/// Returns true if clipboard did not contain any information about nodes or edges.
	bool isEmpty() const;

private:
	void pullDataFromClipboard(QList<NodeInfo> &nodesData, QList<EdgeInfo> &edgesData) const;

	QHash<Id, Id> prepareNodes(models::GraphicalModelAssistApi &graphicalApi
			, QList<NodeInfo> &nodesData, const QPointF &offset);
	QPointF newPos(const NodeInfo &nodeData, const QHash<Id, Id> &copiedIds, const QPointF &offset) const;
	QPointF newGraphicalPos(const NodeInfo &nodeData, const QHash<Id, Id> &copiedIds, const QPointF &offset) const;
	qReal::Id newGraphicalParent(const ElementInfo &data, const QHash<Id, Id> &copiedIds) const;
	QPointF vectorFromContainer(const NodeInfo &nodeData) const;

	void prepareEdge(EdgeInfo &edgeData, const QPointF &offset, const QHash<Id, Id> &copiedIds);

	const bool mIsGraphicalCopy;
	const QPointF mMousePosition;
	const Id mRootDiagramGraphicalId;
	bool mIsEmpty;
};

}
}
