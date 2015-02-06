#include "renameExplosionCommand.h"

#include "models/details/renameDialog.h"
#include "models/logicalModelAssistApi.h"
#include "models/graphicalModelAssistApi.h"

using namespace qReal::commands;

RenameExplosionCommand::RenameExplosionCommand(models::LogicalModelAssistApi &logicalApi
		, models::GraphicalModelAssistApi const * const graphicalApi
		, models::Exploser &exploser
		, const Id &target)
	: mLogicalApi(logicalApi)
	, mGraphicalApi(graphicalApi)
	, mExploser(exploser)
	, mTarget(target)
{
}

RenameExplosionCommand::~RenameExplosionCommand()
{
}

bool RenameExplosionCommand::execute()
{
	if (mNewName.isEmpty()) {
		// Getting here during first execution
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
	mOldName = mLogicalApi.name(mTarget);
	mNewName = gui::RenameDialog::selectNewName(mOldName);
	// Adding real renaming commands; they will be executed just after this
	// command so the sequence is ok
	addPostAction(mExploser.renameCommands(mTarget, mNewName));
}
