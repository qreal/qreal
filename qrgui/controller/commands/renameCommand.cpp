#include "renameCommand.h"

using namespace qReal::commands;

RenameCommand::RenameCommand(details::ModelsAssistInterface * const model
		, Id const &id, QString const &newName)
	: mModel(model)
	, mId(id)
	, mOldName(mModel->name(mId))
	, mNewName(newName)
{
}

RenameCommand::RenameCommand(details::ModelsAssistInterface * const model
		, Id const &id, QString const &oldName, QString const &newName)
	: mModel(model)
	, mId(id)
	, mOldName(oldName)
	, mNewName(newName)
{
}

bool RenameCommand::execute()
{
	return rename(mNewName);
}

bool RenameCommand::restoreState()
{
	return rename(mOldName);
}

bool RenameCommand::rename(QString const &name)
{
	mModel->setName(mId, name);
	return true;
}
