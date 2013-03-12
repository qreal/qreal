#include "removeElementCommand.h"

using namespace qReal::commands;

RemoveElementCommand::RemoveElementCommand(
		models::LogicalModelAssistApi *logicalApi
		, models::GraphicalModelAssistApi *graphicalApi
		, const Id &parent, const Id &id, bool isFromLogicalModel
		, const QString &name, const QPointF &position)
	: mImpl(logicalApi, graphicalApi, parent, id, isFromLogicalModel, name, position)
{
}

bool RemoveElementCommand::execute()
{
	mImpl.remove();
	return true;
}

bool RemoveElementCommand::restoreState()
{
	mImpl.create();
	return true;
}
