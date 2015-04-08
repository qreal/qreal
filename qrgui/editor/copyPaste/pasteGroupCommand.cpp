/* Copyright 2007-2015 QReal Research Group
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

#include <QtWidgets/QApplication>
#include <QtGui/QClipboard>

#include "editor/copyPaste/pasteGroupCommand.h"
#include "editor/copyPaste/pasteNodeCommand.h"
#include "editor/copyPaste/pasteEdgeCommand.h"

using namespace qReal::commands;

PasteGroupCommand::PasteGroupCommand(EditorViewScene *scene
		, bool isGraphicalCopy)
	: mScene(scene)
	, mIsGraphicalCopy(isGraphicalCopy)
	, mIsEmpty(false)
{
	prepareCommands();
}

PasteGroupCommand::~PasteGroupCommand()
{
}

bool PasteGroupCommand::isEmpty() const
{
	return mIsEmpty;
}

void PasteGroupCommand::prepareCommands()
{
	QList<NodeData> nodesData;
	QList<EdgeData> edgesData;
	pullDataFromClipboard(nodesData, edgesData);

	if (nodesData.isEmpty() && edgesData.isEmpty()) {
		mIsEmpty = true;
		return;
	}

	const QPointF originalPos = nodesData.isEmpty() ? edgesData[0].pos : nodesData[0].pos;
	const QPointF offset = mScene->getMousePos() - originalPos;

	mCopiedIds = preparePasteNodesCommands(nodesData, offset);

	foreach (const EdgeData &data, edgesData) {
		preparePasteEdgeCommand(data, offset);
	}
}

QHash<Id, Id> *PasteGroupCommand::preparePasteNodesCommands(QList<NodeData> &nodesData
		, const QPointF &offset)
{
	QHash<Id, Id> *copiedIds = new QHash<Id, Id>();

	while (!nodesData.isEmpty()) {
		const NodeData &nextToPaste = nodesData[0];
		PasteNodeCommand *pasteCommand = new PasteNodeCommand(mScene
				, nextToPaste, offset, mIsGraphicalCopy, copiedIds);
		addPreAction(pasteCommand);
		nodesData.removeAll(nextToPaste);
	}

	return copiedIds;
}

void PasteGroupCommand::preparePasteEdgeCommand(const EdgeData &edgeData, const QPointF &offset)
{
	PasteEdgeCommand *pasteCommand = new PasteEdgeCommand(mScene
			, edgeData, offset, mIsGraphicalCopy, mCopiedIds);
	addPreAction(pasteCommand);
}

bool PasteGroupCommand::execute()
{
	// Execution happens in child commands
	return true;
}

bool PasteGroupCommand::restoreState()
{
	// Restoration happens in child commands
	return true;
}

void PasteGroupCommand::pullDataFromClipboard(QList<NodeData> &nodesData, QList<EdgeData> &edgesData) const
{
	const QClipboard *clipboard = QApplication::clipboard();
	const QMimeData *mimeData = clipboard->mimeData();

	QByteArray data = mimeData->data("application/x-real-uml-model-data");
	QDataStream stream(&data, QIODevice::ReadOnly);

	stream >> nodesData;
	stream >> edgesData;
}
