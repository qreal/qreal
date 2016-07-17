/* Copyright 2012-2016 QReal Research Group
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

#include "models/commands/createElementsCommand.h"
#include "plugins/pluginManager/pattern.h"

namespace qReal {
namespace commands {

/// Creates a group of elements described in some pattern. Command does not execute multiple CreateElementCommand
/// in its implementation, it implements its own logic using optimized AbstractModel::addElementsToModel() version.
/// @see qReal::EditorManagerInteface::pattern().
class QRGUI_MODELS_EXPORT CreatePatternCommand : public CreateElementsCommand
{
public:
	CreatePatternCommand(const models::Models &models, const ElementInfo &pattern);

	Id rootId() const;

protected:
	QList<ElementInfo> parse(models::LogicalModelAssistApi &logicalApi
			, models::GraphicalModelAssistApi &graphicalApi
			, const ElementInfo &pattern);

	Pattern mPattern;
	QMap<QString, Id> mCreatedNodes;
	IdList mCreatedEdges;
	Id mRootId;
};

}
}
