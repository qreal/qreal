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

#include "models/commands/createGroupCommand.h"

namespace qReal {

class EditorViewScene;

namespace commands {

/// Makes same as CreateGroupCommand and automaticly rearranges created items on the scene.
/// In other words this command adds view part into the CreateGroupCommand.
class CreateAndUpdateGroupCommand : public CreateGroupCommand
{
public:
	CreateAndUpdateGroupCommand(EditorViewScene &scene
			, models::LogicalModelAssistApi &logicalApi
			, models::GraphicalModelAssistApi &graphicalApi
			, models::Exploser &exploser
			, const Id &logicalParent
			, const Id &graphicalParent
			, const Id &id
			, bool isFromLogicalModel
			, const QPointF &position);

private:
	bool execute() override;

	EditorViewScene &mScene;
};

}
}
