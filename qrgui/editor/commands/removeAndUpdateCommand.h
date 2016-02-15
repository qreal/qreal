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

#pragma once

#include <qrgui/models/commands/removeElementsCommand.h>

namespace qReal {
namespace gui {
namespace editor {
class EditorViewScene;
namespace commands {

/// Makes same as MultipleRemoveCommand and automaticly rearranges created items on the scene.
/// In other words this command adds view part into the MulipleRemoveCommand.
class RemoveAndUpdateCommand : public qReal::commands::RemoveElementsCommand
{
public:
	RemoveAndUpdateCommand(EditorViewScene &scene, const models::Models &models);

	void appendGraphicalDelete(const Id &id, QList<ElementInfo> &nodes, QList<ElementInfo> &edges) override;

private:
	EditorViewScene &mScene;
};

}
}
}
}
