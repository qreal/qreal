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

#pragma once

#include "controller/commands/abstractCommand.h"
#include "models/logicalModelAssistApi.h"
#include "models/graphicalModelAssistApi.h"

namespace qReal {
namespace commands {

/// Adds or removes explosion between specified elements
class QRGUI_MODELS_EXPORT ExplosionCommand : public AbstractCommand
{
public:
	/// Creates new instance of a command that creates or removes explosions.
	/// If graphical assist api parameter specified then it is allowed to
	/// pass graphical ids as source or target parameters.
	/// Otherwise both source and target must be logical ids.
	/// @param mustAdd Specifies if this command should create or remove explosions.
	ExplosionCommand(models::LogicalModelAssistApi &logicalApi
			, const models::GraphicalModelAssistApi * const graphicalApi
			, const Id &source, const Id &target, bool mustAdd);

	virtual ~ExplosionCommand();

protected:
	virtual bool execute();
	virtual bool restoreState();

private:
	bool processExplosion(bool add);

	models::LogicalModelAssistApi &mLogicalApi;
	const models::GraphicalModelAssistApi * const mGraphicalApi;  // Doesn't take ownership.
	Id mSource;
	Id mTarget;
	const bool mMustAdd;
};

}
}
