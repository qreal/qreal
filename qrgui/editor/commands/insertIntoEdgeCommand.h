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

#include "editor/editorViewScene.h"
#include "models/graphicalModelAssistApi.h"
#include "models/logicalModelAssistApi.h"
#include "models/commands/createElementCommand.h"
#include "models/commands/removeElementCommand.h"

namespace qReal {
namespace commands {

class InsertIntoEdgeCommand : public AbstractCommand
{
public:
	InsertIntoEdgeCommand(EditorViewScene &scene
			, models::LogicalModelAssistApi &logicalAssistApi
			, models::GraphicalModelAssistApi &graphicalAssistApi
			, models::Exploser &exploser
			, const Id &firstElem
			, const Id &lastElem
			, const Id &parent
			, const QPointF &scenePos
			, const QPointF &shift
			, bool isFromLogicalModel
			, CreateElementCommand *createCommand = 0);

	~InsertIntoEdgeCommand();

protected:
	virtual bool execute();
	virtual bool restoreState();

private:
	void initCommand(CreateElementCommand *&command, const Id &type);
	void makeLink(CreateElementCommand *command, NodeElement *src, NodeElement *dst);

	EditorViewScene &mScene;
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

	CreateElementCommand *mCreateFirst;
	CreateElementCommand *mCreateSecond;
	RemoveElementCommand *mRemoveOldEdge;

	CreateElementCommand *mCreateCommand;
};

}
}
