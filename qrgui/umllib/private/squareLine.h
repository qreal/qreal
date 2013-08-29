#pragma once

#include "umllib/private/lineHandler.h"

namespace qReal {

class SquareLine : public LineHandler
{
	Q_OBJECT
public:
	SquareLine(EdgeElement *edge);

	virtual void adjust();

	virtual QPair<QPair<int, qreal>, qreal> arrangeCriteria(NodeElement const *node, QLineF const &portLine) const;

	virtual QList<ContextMenuAction *> extraActions(QPointF const &pos);

protected:
	enum LineType {
		vertical
		, horizontal
		, verticalTurn
		, horizontalTurn
	};

	virtual void handleEdgeMove(QPointF const &pos);
	void moveSegment(QPointF const &oldPos, QPointF const &newPos);

	virtual void improveAppearance();
	void deleteShortSegments();

	QPointF portArrangePoint(NodeElement const *node) const;

	void adjustEndSegments();
	void adjustStart();
	void adjustEnd();

	bool needCorrect() const;
	bool needCorrectSrc() const;
	bool needCorrectDst() const;
	bool intersects(QLineF const &line, NodeElement *node) const;

	void squarize();
	int defineType() const;

	void createHorizontalLine(QPolygonF &line) const;
	void createVerticalLine(QPolygonF &line) const;

	void correctLine(QPolygonF &line, bool correctSrc, bool correctDst, bool isVertical) const;
	void correctVerticalLine(QPolygonF &line, bool correctSrc, bool correctDst) const;
	void correctHorizontalLine(QPolygonF &line, bool correctSrc, bool correctDst) const;

	void createHorizontalInnerLine(QPolygonF &line, bool correctSrc, bool correctDst) const;
	void createVerticalInnerLine(QPolygonF &line, bool correctSrc, bool correctDst) const;
	void verticalInnerLine(QPolygonF &line) const;
	void horizontalInnerLine(QPolygonF &line) const;

	void verticalSquareLine(QPolygonF &line, qreal middleLine) const;
	void horizontalSquareLine(QPolygonF &line, qreal middleLine) const;
	void verticalTurningSquareLine(QPolygonF &line) const;
	void horizontalTurningSquareLine(QPolygonF &line) const;

	void verticalSameSide(QPolygonF &line, bool topLoop) const;
	void horizontalSameSide(QPolygonF &line, bool leftLoop) const;

	QPointF outgoingPoint(bool isStart) const;

	QPair<qreal, qreal> verticalIntermediateSpace() const;
	QPair<qreal, qreal> horizontalIntermediateSpace() const;

	QRectF srcRect() const;
	QRectF dstRect() const;

	virtual void drawPort(QPainter *painter, int portNumber);

	ContextMenuAction mLayOutAction;
};

}
