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

#include <QtCore/QPointF>

#include "qrgui/models/elementInfo.h"

namespace qReal {

namespace models {
class Models;
class LogicalModelAssistApi;
class GraphicalModelAssistApi;
class Exploser;
}

namespace commands {

/// Implements logic for creation and deletion a set of nodes and edges.
class CreateRemoveCommandImplementation
{
public:
	CreateRemoveCommandImplementation(const models::Models &models, const QList<ElementInfo> &elements);

	/// Creates all elements given in constructor. Factical ids of elements after creation may be obtained by results().
	void create();

	/// Removes all elements given in constructor.
	void remove();

	/// Rewrites current elements list that will be created with \a elements.
	void setElements(const QList<ElementInfo> &elements);

	/// Returns information about all elements created or removed by this command.
	/// If creation was performed then result will contain ids that elements got after creation.
	const QList<ElementInfo> &results() const;

	/// @todo: Bad method, required only for linkers. Get rid of it.
	/// Modifies command setting new creation position for the first element info given.
	void setNewPosition(const QPointF &position);

private:
	void removeOne(ElementInfo &element);

	void refreshAllPalettes();

	models::LogicalModelAssistApi &mLogicalApi;
	models::GraphicalModelAssistApi &mGraphicalApi;
	const models::Exploser &mExploser;
	QList<ElementInfo> mElements;
};

}
}
