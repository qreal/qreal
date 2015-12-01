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

#include "foldCommand.h"

using namespace qReal::commands;
using namespace qReal::gui::editor::commands;

FoldCommand::FoldCommand(const NodeElement *element)
	: NodeElementCommand(dynamic_cast<EditorViewScene *>(element->scene()), element->id())
{
}

bool FoldCommand::execute()
{
	NodeElementCommand::execute();
	changeFoldState();
	return true;
}

bool FoldCommand::restoreState()
{
	NodeElementCommand::restoreState();
	changeFoldState();
	return true;
}

void FoldCommand::changeFoldState()
{
	if (mNode) {
		mNode->changeFoldState();
	}
}
