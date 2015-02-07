#include "removeElementCommand.h"

using namespace qReal::commands;

RemoveElementCommand::RemoveElementCommand(
		models::LogicalModelAssistApi &logicalApi
		, models::GraphicalModelAssistApi &graphicalApi
		, const models::Exploser &exploser
		, const Id &logicalParent
		, const Id &graphicalParent
		, const Id &id
		, bool isFromLogicalModel
		, const QString &name
		, const QPointF &position)
	: mImpl(logicalApi, graphicalApi, exploser, logicalParent, graphicalParent
			, id, isFromLogicalModel, name, position)
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

qReal::Id RemoveElementCommand::elementId() const
{
	return mImpl.id();
}

bool RemoveElementCommand::equals(const AbstractCommand &other) const
{
	const RemoveElementCommand *removeCommand = dynamic_cast<const RemoveElementCommand *>(&other);
	if (removeCommand) {
		return mImpl == removeCommand->mImpl;
	}

	return false;
}
