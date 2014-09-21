#pragma once

#include "models/commands/createGroupCommand.h"
#include "editor/editorViewScene.h"

namespace qReal {
namespace commands {

/// Makes same as CreateGroupCommand and automaticly rearranges created items on the scene.
/// In other words this command adds view part into the CreateGroupCommand.
class CreateAndUpdateGroupCommand : public CreateGroupCommand
{
public:
	CreateAndUpdateGroupCommand(EditorViewScene &scene
			, models::LogicalModelAssistApi &logicalApi
			, models::GraphicalModelAssistApi &graphicalApi
			, Exploser &exploser
			, Id const &logicalParent
			, Id const &graphicalParent
			, Id const &id
			, bool isFromLogicalModel
			, QPointF const &position);

private:
	bool execute() override;

	EditorViewScene &mScene;
};

}
}
