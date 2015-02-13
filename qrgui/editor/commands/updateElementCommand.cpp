#include "updateElementCommand.h"

using namespace qReal::commands;

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
