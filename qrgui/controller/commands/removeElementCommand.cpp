#include "removeElementCommand.h"

using namespace qReal::commands;

RemoveElementCommand::RemoveElementCommand(
		models::LogicalModelAssistApi *logicalApi
		, models::GraphicalModelAssistApi *graphicalApi
		, Id const &logicalParent
		, Id const &graphicalParent
		, Id const &id
		, bool isFromLogicalModel
		, QString const &name
		, QPointF const &position)
	: mImpl(logicalApi, graphicalApi, logicalParent, graphicalParent
			, id, isFromLogicalModel, name, position)
{
}

RemoveElementCommand::RemoveElementCommand(
		models::LogicalModelAssistApi *logicalApi
		, models::GraphicalModelAssistApi *graphicalApi
		, Id const &logicalParent
		, Id const &graphicalParent
		, bool isFromLogicalModel
		, Element *element)
	: mImpl(logicalApi, graphicalApi, logicalParent, graphicalParent
			, element->id(), isFromLogicalModel, element->name(), element->pos())
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

bool RemoveElementCommand::equals(AbstractCommand const &other) const
{
	RemoveElementCommand const *removeCommand = dynamic_cast<RemoveElementCommand const *>(&other);
	if (removeCommand) {
		return mImpl == removeCommand->mImpl;
	}
	return false;
}
