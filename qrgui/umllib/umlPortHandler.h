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

	bool checkLowerBorder(QPointF const &point, double const x, double const y) const;
	bool checkUpperBorder(QPointF const &point, double const x, double const y) const;
	bool checkLeftBorder(QPointF const &point, double const x, double const y) const;
	bool checkRightBorder(QPointF const &point, double const x, double const y) const;
	bool checkNoBorderX(QPointF const &point, double const y) const; // TODO: rename
	bool checkNoBorderY(QPointF const &point, double const x) const;

	NodeElement* const mNode;
	bool mBelongsToHorizontalBorders;
};
