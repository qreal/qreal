/* Copyright 2012-2016 Dmitry Mordvinov
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

#include <QtCore/QObject>

#include "qrgui/models/nodeInfo.h"
#include "qrgui/models/edgeInfo.h"
#include "qrgui/models/modelsDeclSpec.h"

namespace qReal {

class Controller;

namespace models {

class Models;

/// Implements pushing a set of elements to clipboard and pasting them to models.
class QRGUI_MODELS_EXPORT Clipboard
{
public:
	Clipboard(Controller &controller, const models::Models &models);

	/// Pushes all relevant data about \a elements into the clipboard.
	void copy(const IdList &elements);

	/// Creates the copy of elements pushed to a clipboard last copy() call in models.
	/// @param parent Graphical id of parent element whoose children will be appended by newly created items.
	/// @param position Upper left corner of items pasted. Relative positions of copied element will be
	/// appended to this value.
	/// @param isGraphicalCopy If true then logical copies are not created, just graphical copies
	/// bound to original logical instances.
	void paste(const Id &parent, const QPointF &position, bool isGraphicalCopy);

	/// Returns true if clipboard does not contain information about QReal diagram elements.
	bool isEmpty() const;

private:
	void pushDataToClipboard(const QList<NodeInfo> &nodesData, const QList<EdgeInfo> &edgesData);
	QList<NodeInfo> nodesData(const IdList &elements);
	QList<EdgeInfo> edgesData(const IdList &elements);
	void addChildren(const Id &node, QList<NodeInfo> &nodes);

	NodeInfo nodeInfo(const Id &node) const;
	EdgeInfo edgeInfo(const Id &edge) const;
	static bool contains(const QList<NodeInfo> &nodes, const Id &id);

	Controller &mController;
	const models::Models &mModels;
};

}
}
