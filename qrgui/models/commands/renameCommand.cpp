#include "renameCommand.h"

#include "models/graphicalModelAssistApi.h"

using namespace qReal::commands;

RenameCommand::RenameCommand(details::ModelsAssistInterface &model
		, const Id &id, const QString &newName
		, const models::Exploser * const exploser)
	: mModel(model)
	, mId(id)
	, mOldName(mModel.name(mId))
	, mNewName(newName)
{
	initExplosions(exploser);
}

RenameCommand::RenameCommand(details::ModelsAssistInterface &model
		, const Id &id, const QString &oldName, const QString &newName
		, const models::Exploser * const exploser)
	: mModel(model)
	, mId(id)
	, mOldName(oldName)
	, mNewName(newName)
{
	initExplosions(exploser);
}

bool RenameCommand::execute()
{
	return rename(mNewName);
}

bool RenameCommand::restoreState()
{
	return rename(mOldName);
}

void RenameCommand::initExplosions(const models::Exploser * const exploser)
{
	if (!exploser) {
		return;
	}

	GraphicalModelAssistInterface *graphicalModel = dynamic_cast<GraphicalModelAssistInterface *>(&mModel);
	const Id logicalId = graphicalModel ? graphicalModel->logicalId(mId) : mId;
	addPostAction(exploser->renameCommands(logicalId, mNewName));
}

bool RenameCommand::rename(const QString &name)
{
	mModel.setName(mId, name);
	return true;
}
