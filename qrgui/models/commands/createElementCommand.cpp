#include "createElementCommand.h"

using namespace qReal::commands;

CreateElementCommand::CreateElementCommand(
		models::LogicalModelAssistApi &logicalApi
		, models::GraphicalModelAssistApi &graphicalApi
		, models::Exploser const &exploser
		, const Id &logicalParent
		, const Id &graphicalParent
		, const Id &id
		, bool isFromLogicalModel
		, const QString &name
		, const QPointF &position)
	: mImpl(logicalApi, graphicalApi, exploser, logicalParent
			, graphicalParent, id, isFromLogicalModel, name, position)
{
}

qReal::Id CreateElementCommand::result() const
{
	return mResult;
}

void CreateElementCommand::setNewPosition(const QPointF &position)
{
	mImpl.setNewPosition(position);
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
