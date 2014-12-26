#include "editor/private/edgeArrangeCriteria.h"

EdgeArrangeCriteria::EdgeArrangeCriteria(int edgeType, qreal yOffset, qreal xOffset)
		: mEdgeType(edgeType)
		, mYOffset(yOffset)
		, mXOffset(xOffset)
{
}

bool EdgeArrangeCriteria::operator<(EdgeArrangeCriteria const &other) const
{
	QPair<int, QPair<qreal, qreal> > first = qMakePair(mEdgeType, qMakePair(mYOffset, mXOffset));
	QPair<int, QPair<qreal, qreal> > second = qMakePair(other.mEdgeType, qMakePair(other.mYOffset, other.mXOffset));

	return first < second;
}
