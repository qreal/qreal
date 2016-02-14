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

class ElementInfo;
namespace models {
class Models;
class Exploser;
class LogicalModelAssistApi;
class GraphicalModelAssistApi;
}

namespace commands {

/// Accepts a list of graphical or logical ids and removes them from model.
class QRGUI_MODELS_EXPORT RemoveElementsCommand : public AbstractCommand
{
public:
	explicit RemoveElementsCommand(const models::Models &models);

	/// Initializes command to remove and restore the given elements.
	/// \a itemsToDelete parameter can`t be passed into the constructor because some virtual methods must be called.
	/// @returns Pointer to itself.
	RemoveElementsCommand *withItemsToDelete(const IdList &itemsToDelete);

	/// Initializes command to remove and restore the given logical element.
	/// \a logicalId parameter can`t be passed into the constructor because some virtual methods must be called.
	/// @returns Pointer to itself.
	RemoveElementsCommand *withLogicalItemToDelete(const Id &logicalId);

	/// Appends to \a nodes and \a edges a list of element infos to remove the logical \a id.
	virtual void appendLogicalDelete(const Id &id, QList<ElementInfo> &nodes, QList<ElementInfo> &edges);

	/// Appends to \a nodes and \a edges a list of element infos to remove the graphical \a id.
	virtual void appendGraphicalDelete(const Id &id, QList<ElementInfo> &nodes, QList<ElementInfo> &edges);

	/// Returns information about all elements removed by this command.
	const QList<ElementInfo> &results() const;

protected:
	bool execute() override;
	bool restoreState() override;

	void appendInfo(QList<ElementInfo> &nodes, QList<ElementInfo> &edges, const ElementInfo &info) const;
	void appendExplosionsCommands(const Id &logicalId, QList<ElementInfo> &nodes, QList<ElementInfo> &edges);
	void appendHangingEdges(QList<ElementInfo> &nodes, QList<ElementInfo> &edges);
	bool contains(QList<ElementInfo> &infos, const Id &id) const;

	models::LogicalModelAssistApi &mLogicalApi;
	models::GraphicalModelAssistApi &mGraphicalApi;
	const models::Exploser &mExploser;
	CreateRemoveCommandImplementation mImpl;
};

}
}
