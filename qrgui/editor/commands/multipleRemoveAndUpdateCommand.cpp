#include "multipleRemoveAndUpdateCommand.h"

#include "editor/editorViewScene.h"
#include "editor/commands/arrangeLinksCommand.h"
#include "editor/commands/updateElementCommand.h"

using namespace qReal::commands;

MultipleRemoveAndUpdateCommand::MultipleRemoveAndUpdateCommand(EditorViewScene &scene
		, models::Models const &models)
	: MultipleRemoveCommand(models)
	, mScene(scene)
{
}

AbstractCommand *MultipleRemoveAndUpdateCommand::graphicalDeleteCommand(const Id &id)
{
	AbstractCommand *result = MultipleRemoveCommand::graphicalDeleteCommand(id);

	// correcting unremoved edges
	ArrangeLinksCommand *arrangeCommand = new ArrangeLinksCommand(&mScene, id, true);
	arrangeCommand->setRedoEnabled(false);
	result->addPreAction(arrangeCommand);

	UpdateElementCommand *updateCommand = new UpdateElementCommand(&mScene, id);
	updateCommand->setRedoEnabled(false);
	result->addPreAction(updateCommand);

	const IdList links = mGraphicalApi.graphicalRepoApi().links(id);
	for (const Id &link : links) {
		UpdateElementCommand *updateLinkCommand = new UpdateElementCommand(&mScene, link);
		updateLinkCommand->setRedoEnabled(false);
		result->addPreAction(updateLinkCommand);
	}

	return result;
}
