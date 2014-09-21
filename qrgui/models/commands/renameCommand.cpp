#include "renameCommand.h"

#include "models/graphicalModelAssistApi.h"

using namespace qReal::commands;

RenameCommand::RenameCommand(details::ModelsAssistInterface &model
		, Id const &id, QString const &newName
		, Exploser const * const exploser)
	: mModel(model)
	, mId(id)
	, mOldName(mModel.name(mId))
	, mNewName(newName)
{
	initExplosions(exploser);
}

RenameCommand::RenameCommand(details::ModelsAssistInterface &model
		, Id const &id, QString const &oldName, QString const &newName
		, Exploser const * const exploser)
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

void RenameCommand::initExplosions(Exploser const * const exploser)
{
	if (!exploser) {
		return;
	}

	GraphicalModelAssistInterface *graphicalModel = dynamic_cast<GraphicalModelAssistInterface *>(&mModel);
	Id const logicalId = graphicalModel ? graphicalModel->logicalId(mId) : mId;
	addPostAction(exploser->renameCommands(logicalId, mNewName));
}

bool RenameCommand::rename(QString const &name)
{
	mModel.setName(mId, name);
	return true;
}
