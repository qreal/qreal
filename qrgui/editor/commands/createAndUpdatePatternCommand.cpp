/* Copyright 2014-2016 Dmitry Mordvinov
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

#include "createAndUpdatePatternCommand.h"

#include "editor/editorViewScene.h"
#include "editor/commands/insertIntoEdgeCommand.h"

using namespace qReal::gui::editor::commands;

CreateAndUpdatePatternCommand::CreateAndUpdatePatternCommand(EditorViewScene &scene
		, const models::Models &models, const ElementInfo &pattern)
	: CreatePatternCommand(models, pattern)
	, mScene(scene)
{
	InsertIntoEdgeCommand * const insertCommand = new InsertIntoEdgeCommand(mScene, mModels
			, mCreatedNodes[mPattern.inNode()]
			, mCreatedNodes[mPattern.outNode()]
			, pattern.graphicalParent(), pattern.position()
			, mPattern.size(), pattern.logicalId() == pattern.id());

	insertCommand->setUndoEnabled(false);
	addPostAction(insertCommand);
}

bool CreateAndUpdatePatternCommand::execute()
{
	if (!CreatePatternCommand::execute()) {
		return false;
	}

	for (const Id &edge : mCreatedEdges) {
		mScene.reConnectLink(mScene.getEdgeById(edge));
	}

	return true;
}
