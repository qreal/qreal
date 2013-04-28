#pragma once

#include <QtCore/QRectF>
#include <QtCore/QPair>
#include <QtWidgets/QGraphicsGridLayout>

#include "gridIndex.h"

namespace qReal
{
namespace layouts
{

/// Provides a method for grid approximation clusterizing gives collection
/// of rectangles by some grid with best movement cost
class GridApproximizer
{
public:
	/// first = segment begin, second = segment end
	struct Segment : public QPair<int, int>
	{
	public:
		Segment(int begin, int end);
		int length() const;
		inline bool operator<(Segment const &rhs) const;
	};

	static void placeItems(QGraphicsGridLayout * const layout
		, QList<QGraphicsLayoutItem *> const &items);

private:
	/// Performs grid approximation with given collection
	/// of rectangles clusterization with best movement cost
	static QMap<QRect, GridIndex> performApproximation(QList<QRect> const &rects);

	/// Performs segments relative layout approximation.
	/// Simly sorts segment clusters.
	static QMap<Segment, QList<int> > performLinearApproximation(QList<Segment> const &segments);

	/// Performs segments clusterization using a condition that
	/// two segments belong to one cluster iff they intersect
	/// at least on a half length of at least one of them
	static QList<QSet<Segment> > performLinearClusterization(QList<Segment> const &segments);

	/// Tells if specified linear segments intersect region length is a half
	/// or more of length of at least on of them
	static bool intersectsOnHalfOrMore(Segment const &segment1, Segment const &segment2);

	static int intersectionLength(Segment const &segment1, Segment const &segment2);

	static bool canAdd(QSet<Segment> const &cluster, Segment const &segment
			, QMap<Segment, QMap<Segment, bool> > const &adjucencyMatrix);

	static int minimum(QList<int> const &list);
};

// Small hack for storing into QMap
inline bool operator<(QRect const &lhs, QRect const &rhs)
{
	return lhs.x() != rhs.x() ? lhs.x() < rhs.x()
			: lhs.y() != rhs.y() ? lhs.y() < rhs.y()
			: lhs.width() != rhs.width() ? lhs.width() < rhs.width()
			: lhs.height() < rhs.height();
}

}
}
