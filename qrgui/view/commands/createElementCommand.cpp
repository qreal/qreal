#include "createElementCommand.h"

using namespace qReal::commands;

CreateElementCommand::CreateElementCommand(models::GraphicalModelAssistApi *api
		, const Id &parent, const Id &id, bool isFromLogicalModel
		, const QString &name, const QPointF &position)
	: mImpl(api, parent, id, isFromLogicalModel, name, position)
{
}

qReal::Id CreateElementCommand::result() const
{
	return mResult;
}

bool CreateElementCommand::execute()
{
	mResult = mImpl.create();
	return true;
}

bool CreateElementCommand::restoreState()
{
	mImpl.remove();
	return true;
}
