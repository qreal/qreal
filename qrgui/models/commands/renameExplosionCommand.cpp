/* Copyright 2013-2016 Dmitry Mordvinov
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

#include "renameExplosionCommand.h"

#include "models/details/renameDialog.h"
#include "models/logicalModelAssistApi.h"
#include "models/graphicalModelAssistApi.h"

using namespace qReal::commands;

RenameExplosionCommand::RenameExplosionCommand(models::LogicalModelAssistApi &logicalApi
		, const models::GraphicalModelAssistApi * const graphicalApi
		, const models::Exploser &exploser
		, const Id &target)
	: mLogicalApi(logicalApi)
	, mGraphicalApi(graphicalApi)
	, mExploser(exploser)
	, mTarget(target)
{
}

RenameExplosionCommand::~RenameExplosionCommand()
{
}

bool RenameExplosionCommand::execute()
{
	if (mNewName.isEmpty()) {
		// Getting here during first execution
		promptUserToEnterNewName();
	}

	return true;
}

bool RenameExplosionCommand::restoreState()
{
	return true;
}

void RenameExplosionCommand::ensureLogicalId()
{
	if (mGraphicalApi && mGraphicalApi->isGraphicalId(mTarget)) {
		mTarget = mGraphicalApi->logicalId(mTarget);
	}
}

void RenameExplosionCommand::promptUserToEnterNewName()
{
	ensureLogicalId();
	mOldName = mLogicalApi.name(mTarget);
	mNewName = gui::RenameDialog::selectNewName(mOldName);
	// Adding real renaming commands; they will be executed just after this
	// command so the sequence is ok
	addPostAction(mExploser.renameCommands(mTarget, mNewName));
}
