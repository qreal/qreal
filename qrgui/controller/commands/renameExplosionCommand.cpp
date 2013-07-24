#include "renameExplosionCommand.h"
#include "../../dialogs/renameDialog.h"

using namespace qReal::commands;

RenameExplosionCommand::RenameExplosionCommand(models::LogicalModelAssistApi * const logicalApi
		, models::GraphicalModelAssistApi const * const graphicalApi
		, Id const &target)
	: mLogicalApi(logicalApi)
	, mGraphicalApi(graphicalApi)
	, mTarget(target)
{
}

RenameExplosionCommand::~RenameExplosionCommand()
{
}

bool RenameExplosionCommand::execute()
{
	if (mNewName.isEmpty()) {
		promptUserToEnterNewName();
	}
	return true;
}

bool RenameExplosionCommand::restoreState()
{
	return true;
}

void RenameExplosionCommand::ensureLogicalId()
{
	if (mGraphicalApi && mGraphicalApi->isGraphicalId(mTarget)) {
		mTarget = mGraphicalApi->logicalId(mTarget);
	}
}

void RenameExplosionCommand::promptUserToEnterNewName()
{
	ensureLogicalId();
	mOldName = mLogicalApi->name(mTarget);
	mLogicalApi->name(mTarget);
	mNewName = gui::RenameDialog::selectNewName(mOldName);
	addPostAction(mLogicalApi->exploser().renameCommands(mTarget, mNewName));
}
