#include "changeParentCommand.h"

using namespace qReal::commands;

ChangeParentCommand::ChangeParentCommand(models::LogicalModelAssistApi &logicalApi
		, models::GraphicalModelAssistApi &graphicalApi
		, bool isLogical, const Id &id, const Id &oldParent
		, const Id newParent, const QPointF &oldPosition
		, const QPointF &newPosition)
	: mLogicalApi(logicalApi)
	, mGraphicalApi(graphicalApi)
	, mIsLogical(isLogical)
	, mId(id)
	, mOldParent(oldParent)
	, mNewParent(newParent)
	, mOldPosition(oldPosition)
	, mNewPosition(newPosition)
{
}

bool ChangeParentCommand::execute()
{
	changeParent(mNewParent, mNewPosition);
	return true;
}

bool ChangeParentCommand::restoreState()
{
	changeParent(mOldParent, mOldPosition);
	return true;
}

void ChangeParentCommand::changeParent(const qReal::Id &parent, const QPointF &position)
{
	if (mIsLogical) {
		mLogicalApi.changeParent(mId, parent, position);
	} else {
		mGraphicalApi.changeParent(mId, parent, position);
	}
}
