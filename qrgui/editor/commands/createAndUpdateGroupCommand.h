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
