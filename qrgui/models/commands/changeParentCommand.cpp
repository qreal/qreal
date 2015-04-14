/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
