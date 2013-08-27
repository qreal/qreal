#pragma once

#include "lineHandler.h"

namespace qReal {

class SquareLine : public LineHandler
{
public:
	SquareLine(EdgeElement *edge);

	virtual void adjust();

	virtual QPair<QPair<int, qreal>, qreal> arrangeCriteria(NodeElement const *node, QLineF const &portLine) const;

protected:
	enum LineType {
		vertical
		, horizontal
		, verticalTurn
		, horizontalTurn
	};

	virtual void handleEdgeMove(QPointF const &pos, bool needAlign);
	virtual void improveAppearance();
	void moveSegment(QPointF const &oldPos, QPointF const &newPos);

	QPointF portArrangePoint(NodeElement const *node) const;

	void adjustEndSegments();
	void adjustStart();
	void adjustEnd();

	bool needCorrect() const;
	bool needCorrectSrc() const;
	bool needCorrectDst() const;
	bool intersects(QLineF const &line, NodeElement *node) const;

	void squarize();
	int defineType();

	void verticalSquareLine(QPolygonF &line, qreal middle);
	void horizontalSquareLine(QPolygonF &line, qreal middle);
	void verticalTurningSquareLine(QPolygonF &line);
	void horizontalTurningSquareLine(QPolygonF &line);

	void verticalSameSide(QPolygonF &line, bool topLoop);
	void horizontalSameSide(QPolygonF &line, bool leftLoop);
	QPointF outgoingPoint(bool isStart) const;

	QPair<qreal, qreal> verticalIntermediateSpace() const;
	QPair<qreal, qreal> horizontalIntermediateSpace() const;
};

}
