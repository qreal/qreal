/** @file umlPortHandler.h
 * 	@brief Class for handling port item behaviour at move events.
 * 	Changes port position, parent if it is necessary.
 * */

#pragma once

#include <QtCore/QPointF>

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
	void handleMoveEvent(bool const leftPressed
			, QPointF &pos, QPointF const &scenePos
			, NodeElement *&parentNode);

private:
	void handleHorizontalBorders(const NodeElement * const tmpNode, const NodeElement * const parentNode
			, QPointF const &pos, QPointF const &posInItem) const;
	// whatever it means
	// TODO: rename

	NodeElement * const mNode;
	bool mBelongsToHorizontalBorders;
};
