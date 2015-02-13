#pragma once

#include <QtCore/QPair>

class EdgeArrangeCriteria
{
public:
	EdgeArrangeCriteria(int edgeType, qreal yOffset, qreal xOffset);

	bool operator<(const EdgeArrangeCriteria &other) const;

private:
	int mEdgeType;
	qreal mYOffset;
	qreal mXOffset;
};

