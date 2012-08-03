#pragma once

#include <QtCore/QPointF>

class NodeElement;

class UmlPortHandler
{
public:
	UmlPortHandler(NodeElement* const node);

	void handleMoveEvent(bool const leftPressed
			, QPointF &pos, QPointF const &scenePos
			, NodeElement *&parentNode);

private:
	void handleHorizontalBorders(const NodeElement* const tmpNode, const NodeElement* const parentNode
			, QPointF const &pos, QPointF const &posInItem) const;
	// whatever it means
	// TODO: rename

	NodeElement* const mNode;
	bool mBelongsToHorizontalBorders;
};
