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

#include "createAndUpdateGroupCommand.h"

#include "editor/editorViewScene.h"
#include "editor/commands/insertIntoEdgeCommand.h"

using namespace qReal::commands;

CreateAndUpdateGroupCommand::CreateAndUpdateGroupCommand(EditorViewScene &scene
		, models::LogicalModelAssistApi &logicalApi
		, models::GraphicalModelAssistApi &graphicalApi
		, models::Exploser &exploser
		, const Id &logicalParent
		, const Id &graphicalParent
		, const Id &id
		, bool isFromLogicalModel
		, const QPointF &position)
	: CreateGroupCommand(logicalApi, graphicalApi, exploser, logicalParent
			, graphicalParent, id, isFromLogicalModel, position)
	, mScene(scene)
{
	InsertIntoEdgeCommand * const insertCommand = new InsertIntoEdgeCommand(mScene, mLogicalApi, mGraphicalApi
			, mExploser, mNodeCommands[mPattern.inNode()]->result(), mNodeCommands[mPattern.outNode()]->result()
			, mGraphicalParent, mPosition, mPattern.size(), mIsFromLogicalModel);
	insertCommand->setUndoEnabled(false);
	addPostAction(insertCommand);
}

bool CreateAndUpdateGroupCommand::execute()
{
	if (!CreateGroupCommand::execute()) {
		return false;
	}

	for (int i = 0; i < mEdgeCommands.count(); ++i) {
		mScene.reConnectLink(mScene.getEdgeById(mEdgeCommands[i]->result()));
	}

	return true;
}
