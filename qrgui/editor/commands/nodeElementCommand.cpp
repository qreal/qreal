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

#include "nodeElementCommand.h"

using namespace qReal::gui::editor;
using namespace qReal::gui::editor::commands;

NodeElementCommand::NodeElementCommand(const EditorViewScene *scene, const Id &id)
	: ElementCommand(scene, id)
{
}

NodeElementCommand::NodeElementCommand(const EditorView *view, const Id &id)
	: ElementCommand(&view->editorViewScene(), id)
{
}

NodeElementCommand::~NodeElementCommand()
{
}

bool NodeElementCommand::reinitElement()
{
	if (!ElementCommand::reinitElement()) {
		return false;
	}

	mNode = dynamic_cast<NodeElement *>(mElement);
	return mNode != nullptr;
}

NodeElement *NodeElementCommand::nodeById(const Id &id)
{
	return dynamic_cast<NodeElement *>(elementById(id));
}
