#include "updateElementCommand.h"

using namespace qReal::commands;

UpdateElementCommand::UpdateElementCommand(EditorViewScene const *scene, Id const &id)
	: ElementCommand(scene, id)
{
}

UpdateElementCommand::UpdateElementCommand(EditorView const *view, Id const &id)
	: ElementCommand(view->editorViewScene(), id)
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

bool UpdateElementCommand::equals(AbstractCommand const &other) const
{
	UpdateElementCommand const *updateCommand = dynamic_cast<UpdateElementCommand const *>(&other);
	if (updateCommand) {
		return scene() == updateCommand->scene() && elementId() == updateCommand->elementId();
	}
	return false;
}
