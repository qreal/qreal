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
