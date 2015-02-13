/** @file umlPortHandler.h
* 	@brief Class for handling port item behaviour at move events.
* 	Changes port position, parent if it is necessary.
**/

#pragma once

#include <QtCore/QPointF>

namespace qReal {

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
