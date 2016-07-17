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

class QRGUI_MODELS_EXPORT ChangeParentCommand : public AbstractCommand
{
public:
	ChangeParentCommand(
			models::LogicalModelAssistApi &logicalApi
			, models::GraphicalModelAssistApi &graphicalApi
			, bool isLogical, const Id &id, const Id &oldParent
			, const Id newParent, const QPointF &oldPosition
			, const QPointF &newPosition);

protected:
	bool execute();
	bool restoreState();

private:
	void changeParent(const Id &parent, const QPointF &position);

	models::LogicalModelAssistApi &mLogicalApi;
	models::GraphicalModelAssistApi &mGraphicalApi;
	const bool mIsLogical;
	const Id mId;
	const Id mOldParent;
	const Id mNewParent;
	const QPointF mOldPosition;
	const QPointF mNewPosition;
};

}
}
