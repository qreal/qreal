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

#pragma once

#include "editor/editorViewScene.h"
#include "models/graphicalModelAssistApi.h"
#include "models/logicalModelAssistApi.h"
#include "models/commands/createElementsCommand.h"
#include "models/commands/removeElementsCommand.h"

namespace qReal {
namespace gui {
namespace editor {
namespace commands {

class InsertIntoEdgeCommand : public qReal::commands::AbstractCommand
{
public:
	InsertIntoEdgeCommand(EditorViewScene &scene
			, const models::Models &models
			, const Id &firstElem
			, const Id &lastElem
			, const Id &parent
			, const QPointF &scenePos
			, const QPointF &shift
			, bool isFromLogicalModel
			, qReal::commands::CreateElementsCommand *createCommand = 0);

	~InsertIntoEdgeCommand();

protected:
	virtual bool execute();
	virtual bool restoreState();

private:
	void initCommand(qReal::commands::CreateElementsCommand *&command, const Id &type);
	void makeLink(qReal::commands::CreateElementsCommand *command, NodeElement *src, NodeElement *dst);

	EditorViewScene &mScene;
	const models::Models &mModels;
	models::LogicalModelAssistApi &mLogicalAssistApi;
	models::GraphicalModelAssistApi &mGraphicalAssistApi;
	models::Exploser &mExploser;

	Id mFirstId;
	Id mLastId;
	Id mParentId;

	Id mOldEdge;

	QPointF mPos;
	QPolygon mConfiguration;
	QPointF mShift;

	bool mIsLogical;

	QMap<Id, QPointF> mElementShifting;

	qReal::commands::CreateElementsCommand *mCreateFirst;
	qReal::commands::CreateElementsCommand *mCreateSecond;
	qReal::commands::RemoveElementsCommand *mRemoveOldEdge;

	qReal::commands::CreateElementsCommand *mCreateCommand;
};

}
}
}
}
