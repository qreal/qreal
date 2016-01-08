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

#include "removeAndUpdateCommand.h"

#include <models/models.h>

#include "editor/editorViewScene.h"
#include "editor/commands/arrangeLinksCommand.h"
#include "editor/commands/updateElementCommand.h"

using namespace qReal::commands;
using namespace qReal::gui::editor::commands;

RemoveAndUpdateCommand::RemoveAndUpdateCommand(EditorViewScene &scene
		, const models::Models &models)
	: RemoveElementsCommand(models)
	, mScene(scene)
{
}

void RemoveAndUpdateCommand::appendGraphicalDelete(const Id &id
		, QList<ElementInfo> &nodes, QList<ElementInfo> &edges)
{
	RemoveElementsCommand::appendGraphicalDelete(id, nodes, edges);

	// correcting unremoved edges
	ArrangeLinksCommand *arrangeCommand = new ArrangeLinksCommand(&mScene, id, true);
	arrangeCommand->setRedoEnabled(false);
	addPreAction(arrangeCommand);

	UpdateElementCommand *updateCommand = new UpdateElementCommand(&mScene, id);
	updateCommand->setRedoEnabled(false);
	addPreAction(updateCommand);

	const IdList links = mGraphicalApi.graphicalRepoApi().links(id);
	for (const Id &link : links) {
		UpdateElementCommand *updateLinkCommand = new UpdateElementCommand(&mScene, link);
		updateLinkCommand->setRedoEnabled(false);
		addPreAction(updateLinkCommand);
	}
}
