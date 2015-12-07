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

/** @file umlPortHandler.h
* 	@brief Class for handling port item behaviour at move events.
* 	Changes port position, parent if it is necessary.
**/

#pragma once

#include <QtCore/QPointF>

namespace qReal {
namespace gui {
namespace editor {

class NodeElement;

class UmlPortHandler
{
public:
	/**
	 * Constructs a UmlPortHandler.
	 * @param node Node that is actually dealt with.
	 */
	UmlPortHandler(NodeElement * const node);

	/**
	 * Handles NodeElement behaviour (sets its position and parent).
	 * @param leftPressed Shows is left mouse button pressed of not.
	 */
	void handleMoveEvent(const bool leftPressed
			, QPointF &pos, const QPointF &scenePos
			, NodeElement *&parentNode);

private:
	void handleHorizontalBorders(const NodeElement * const tmpNode, const NodeElement * const parentNode
			, const QPointF &pos, const QPointF &posInItem) const;
	// whatever it means
	// TODO: rename

	NodeElement * const mNode;
	bool mBelongsToHorizontalBorders;
};

}
}
}
