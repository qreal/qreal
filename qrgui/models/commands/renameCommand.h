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
#include "models/exploser.h"
#include "plugins/toolPluginInterface/usedInterfaces/details/modelsAssistInterface.h"

namespace qReal {
namespace commands {

/// Assigns to element with given id given name using given model (logical or graphical)
class QRGUI_MODELS_EXPORT RenameCommand : public AbstractCommand
{
public:
	/// This overload gets old name automatically using models api
	RenameCommand(details::ModelsAssistInterface &model
			, const Id &id, const QString &newName
			, const models::Exploser * const exploser = nullptr);

	/// This overload alows to specify old name manually
	RenameCommand(details::ModelsAssistInterface &model
			, const Id &id, const QString &oldName, const QString &newName
			, const models::Exploser * const exploser = nullptr);

protected:
	virtual bool execute();
	virtual bool restoreState();

private:
	void initExplosions(const models::Exploser * const exploser);

	bool rename(const QString &name);

	details::ModelsAssistInterface &mModel;
	const Id mId;
	const QString mOldName;
	const QString mNewName;
};

}
}
