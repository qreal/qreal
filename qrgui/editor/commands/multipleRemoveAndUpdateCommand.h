#pragma once

#include "models/commands/multipleRemoveCommand.h"

namespace qReal {

class EditorViewScene;

namespace commands {

/// Makes same as MultipleRemoveCommand and automaticly rearranges created items on the scene.
/// In other words this command adds view part into the MulipleRemoveCommand.
class MultipleRemoveAndUpdateCommand : public MultipleRemoveCommand
{
public:
	MultipleRemoveAndUpdateCommand(EditorViewScene &scene, const models::Models &models);

private:
	AbstractCommand *graphicalDeleteCommand(const Id &id) override;

	EditorViewScene &mScene;
};

}
}
