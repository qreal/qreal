#pragma once

#include <QtCore/QPointF>

class NodeElement;

class UmlPortHandler
{
public:
	UmlPortHandler(NodeElement *node);

	void handleMoveEvent(bool const leftPressed, QPointF &pos, QPointF const scenePos
						 , NodeElement *&parentNode);

private:
	void handleHorizontalBorders(NodeElement *tmpNode, NodeElement *parentNode, QPointF &pos, QPointF &posInItem) const;
	// whatever it means
	// TODO: rename

	NodeElement *mNode;
	bool mBelongsToHorizontalBorders;
};
