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

#pragma once

#include "controller/commands/abstractCommand.h"
#include "editor/editorViewScene.h"

namespace qReal {
namespace commands {

class PasteGroupCommand : public AbstractCommand
{
public:
	PasteGroupCommand(EditorViewScene *scene, bool isGraphicalCopy);
	virtual ~PasteGroupCommand();

	bool isEmpty() const;

protected:
	bool execute();
	bool restoreState();

private:
	void prepareCommands();
	QHash<Id, Id> *preparePasteNodesCommands(QList<NodeData> &nodesData, const QPointF &offset);
	void preparePasteEdgeCommand(const EdgeData &edgeData, const QPointF &offset);

	void pullDataFromClipboard(QList<NodeData> &nodesData, QList<EdgeData> &edgesData) const;

	EditorViewScene *mScene;
	const bool mIsGraphicalCopy;
	QHash<Id, Id> *mCopiedIds;
	bool mIsEmpty;
};

}
}
