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

#include "updateElementCommand.h"

using namespace qReal::gui::editor::commands;

UpdateElementCommand::UpdateElementCommand(const EditorViewScene *scene, const Id &id)
	: ElementCommand(scene, id)
{
}

UpdateElementCommand::UpdateElementCommand(const EditorView *view, const Id &id)
	: ElementCommand(&view->editorViewScene(), id)
{
}

UpdateElementCommand::~UpdateElementCommand()
{
}

bool UpdateElementCommand::execute()
{
	return ElementCommand::execute() && update();
}

bool UpdateElementCommand::restoreState()
{
	return ElementCommand::restoreState() && update();
}

bool UpdateElementCommand::update()
{
	if (mElement) {
		// TODO: Edge element updateData() has field mModelUpdateIsCalled
		// that blocks first call. Need to do something with it not to call
		// updateData() twice
		mElement->updateData();
		mElement->updateData();
		mElement->connectToPort();
		mElement->checkConnectionsToPort();
		return true;
	}

	return false;
}

bool UpdateElementCommand::equals(const AbstractCommand &other) const
{
	const UpdateElementCommand *updateCommand = dynamic_cast<const UpdateElementCommand *>(&other);
	if (updateCommand) {
		return scene() == updateCommand->scene() && elementId() == updateCommand->elementId();
	}

	return false;
}
