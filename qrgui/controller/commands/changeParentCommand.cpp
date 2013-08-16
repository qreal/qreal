#include "changeParentCommand.h"

using namespace qReal::commands;

ChangeParentCommand::ChangeParentCommand(models::LogicalModelAssistApi &logicalApi
		, models::GraphicalModelAssistApi &graphicalApi
		, bool isLogical, Id const &id, Id const &oldParent
		, Id const newParent, QPointF const &oldPosition
		, QPointF const &newPosition)
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

void ChangeParentCommand::changeParent(qReal::Id const &parent, QPointF const &position)
{
	if (mIsLogical) {
		mLogicalApi.changeParent(mId, parent, position);
	} else {
		mGraphicalApi.changeParent(mId, parent, position);
	}
}
