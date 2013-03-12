#include "createElementCommand.h"

using namespace qReal::commands;

CreateElementCommand::CreateElementCommand(
		models::LogicalModelAssistApi *logicalApi
		, models::GraphicalModelAssistApi *graphicalApi
		, Id const &parent
		, Id const &id
		, bool isFromLogicalModel
		, QString const &name
		, QPointF const &position)
	: mImpl(logicalApi, graphicalApi, parent, id, isFromLogicalModel, name, position)
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
