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

#pragma once

#include <qrgui/controller/commands/abstractCommand.h>

#include "qrgui/models/modelsDeclSpec.h"
#include "qrgui/models/commands/createRemoveCommandImplementation.h"

namespace qReal {
namespace commands {

class QRGUI_MODELS_EXPORT CreateElementsCommand : public AbstractCommand
{
public:
	CreateElementsCommand(const models::Models &models, const QList<ElementInfo> &elements);

	/// Returns information about all elements created by this command.
	/// Results will contain factical ids that elements got after creation (they may differ from given in constructor).
	const QList<ElementInfo> &results() const;

	/// @todo: Bad method, required only for linkers. Get rid of it.
	/// Modifies command setting new creation position for the first element info given.
	void setNewPosition(const QPointF &position);

protected:
	void setElements(const QList<ElementInfo> &elements);
	void addExplosionCommands(const QList<ElementInfo> &elements);

	bool execute() override;
	bool restoreState() override;

	const models::Models &mModels;
	models::LogicalModelAssistApi &mLogicalApi;
	models::GraphicalModelAssistApi &mGraphicalApi;
	const models::Exploser &mExploser;
	CreateRemoveCommandImplementation mImpl;
};

}
}
