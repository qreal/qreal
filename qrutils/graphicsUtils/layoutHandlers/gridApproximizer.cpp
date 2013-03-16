#include <QtCore/QMap>
#include <QtCore/QSet>

#include "gridApproximizer.h"

using namespace qReal::layouts;

void GridApproximizer::placeItems(QGraphicsGridLayout * const layout
	, QList<QGraphicsLayoutItem *> const &items)
{
	QList<QRect> geometryRects;
	foreach (QGraphicsLayoutItem const *item, items) {
		geometryRects << item->geometry().toRect();
	}

	QMap<QRect, GridIndex> const gridIndexes = performApproximation(geometryRects);
	QList<GridIndex> associatedGridIndexes;
	// Now we must place items in correct order (sequentially)
	foreach (QGraphicsLayoutItem * const item, items) {
		GridIndex index(gridIndexes[item->geometry().toRect()]);
		index.bindWithItem(item);
		associatedGridIndexes << index;
	}
	qSort(associatedGridIndexes);
	foreach (GridIndex const &index, associatedGridIndexes) {
		GridIndex nonconstantCopy(index);
		nonconstantCopy.place(layout);
	}
}

// Cannot use QRectF because Qt does not support hashes from qreal (in QSet)
QMap<QRect, GridIndex> GridApproximizer::performApproximation(QList<QRect> const &rects)
{
	QList<Segment> horizontalSegments;
	QList<Segment> verticalSegments;
	foreach (QRect const &rect, rects) {
		horizontalSegments << Segment(rect.left(), rect.right());
		verticalSegments << Segment(rect.top(), rect.bottom());
	}
	QMap<Segment, QList<int> > const horizontalClusters = performLinearApproximation(horizontalSegments);
	QMap<Segment, QList<int> > const verticalClusters = performLinearApproximation(verticalSegments);

	QMap<QRect, GridIndex> result;
	foreach (QRect const &rect, rects) {
		QList<int> const columns = horizontalClusters[Segment(rect.left(), rect.right())];
		QList<int> const rows = verticalClusters[Segment(rect.top(), rect.bottom())];
		int const minColumn = minimum(columns);
		int const minRow = minimum(rows);
		result.insert(rect, GridIndex(minRow, minColumn, columns.count(), rows.count()));
	}

	return result;
}

bool representersComparator(QPair<int, int> const representer1
		, QPair<int, int> const representer2)
{
	return representer1.first < representer2.first;
}

QMap<GridApproximizer::Segment, QList<int> > GridApproximizer::performLinearApproximation(
		QList<Segment> const &segments)
{
	QList<QSet<Segment> > const clusters = performLinearClusterization(segments);

	// Building reverse structure. It will be useful in representers search and
	// result building
	QMap<Segment, QList<int> > segmentClusterMap;
	foreach (Segment const &segment, segments) {
		segmentClusterMap.insert(segment, QList<int>());
	}
	for (int cluster = 0; cluster < clusters.count(); ++cluster) {
		QSet<Segment> const clusterSet = clusters[cluster];
		foreach (Segment const &segment, clusterSet) {
			segmentClusterMap[segment] << cluster;
		}
	}

	// Our purpose here is to tell what clusters each segment belongs to
	// Segments must be enumerated in their x-coordinate increasing order

	QList<QPair<int, int> > representers;
	// By definition each cluster contains representer who belongs JUST
	// to that cluster. Taking such represoter from each cluster
	for (int index = 0; index < clusters.count(); ++index) {
		QSet<Segment> const &cluster = clusters[index];

		// Searching for leftmost segment in cluster
		Segment uniqueRepresenter(INT_MAX, INT_MAX);
		foreach (Segment const &representer, cluster) {
			if (segmentClusterMap[representer].count() == 1) {
				uniqueRepresenter = representer;
				break;
			}
		}

		// Pushing just first coordinate because only it compared
		representers << QPair<int, int>(uniqueRepresenter.first, index);
	}
	qSort(representers.begin(), representers.end(), representersComparator);

	// Now we have sorted representers list. We want to know clusters
	// order numbers
	QMap<int, int > clustersOrderNumbers;
	for (int orderNumber = 0; orderNumber < representers.count(); ++orderNumber) {
		clustersOrderNumbers.insert(representers[orderNumber].second, orderNumber);
	}

	// We alredy built map segments to clusters. Now we just can replace
	// clusters indexes to their order numbers and return it
	foreach (Segment const &segment, segments) {
		QList<int> indexes = segmentClusterMap[segment];
		for (int i = 0; i < indexes.count(); ++i) {
			indexes[i] = clustersOrderNumbers[indexes[i]];
		}
		segmentClusterMap.insert(segment, indexes);
	}
	return segmentClusterMap;
}

QList<QSet<GridApproximizer::Segment> > GridApproximizer::performLinearClusterization(
		QList<Segment> const &segments)
{
	QList<QSet<Segment> > clusters;
	foreach (Segment const &segment, segments) {
		QSet<Segment> currentCluster;
		currentCluster << segment;
		clusters << currentCluster;
	}

	// Building adjacency matrix of segment intersection graph
	QMap<Segment, QMap<Segment, bool> > intersectsOnHalf;
	int const count = segments.count();
	for (int i = 0; i < count - 1; ++i) {
		Segment const segment1 = segments[i];
		for (int j = i + 1; j < count; ++j) {
			Segment const segment2 = segments[j];
			bool currentRelation = intersectsOnHalfOrMore(segment1, segment2);
			intersectsOnHalf[segment1].insert(segment2, currentRelation);
			intersectsOnHalf[segment2].insert(segment1, currentRelation);
		}
	}

	// A cluster is a full subgraph of our graph. Seaching for all clusters
	// adding segments into each while we can

	bool somethingChanged;
	do {
		somethingChanged = false;
		foreach (QSet<Segment> const &cluster, clusters) {
			foreach (Segment const &segment, segments) {
				if (canAdd(cluster, segment, intersectsOnHalf)) {
					somethingChanged = true;
					clusters.removeAll(cluster);
					QSet<Segment> newCluster(cluster);
					newCluster << segment;
					// operator== would work
					if (!clusters.contains(newCluster)) {
						clusters << newCluster;
					}
				}
			}
		}
	} while (somethingChanged);

	return clusters;
}

bool GridApproximizer::intersectsOnHalfOrMore(Segment const &segment1, Segment const &segment2)
{
	int const intersection = intersectionLength(segment1, segment2);
	return 2 * intersection >= segment1.length()
			|| 2 * intersection >= segment2.length();
}

int GridApproximizer::intersectionLength(Segment const &segment1, Segment const &segment2)
{
	Segment const leftSegment = segment1 < segment2 ? segment1 : segment2;
	Segment const rightSegment = segment1 == leftSegment ? segment2 : segment1;
	return qMax(leftSegment.second - rightSegment.first, 0);
}

bool GridApproximizer::canAdd(QSet<Segment> const &cluster, Segment const &segment
		, QMap<Segment, QMap<Segment, bool> > const &adjucencyMatrix)
{
	if (cluster.contains(segment)) {
		return false;
	}
	// Checking if our segment connected to each element in cluster
	foreach (Segment const &clusterElement, cluster) {
		if (!adjucencyMatrix[clusterElement][segment]) {
			return false;
		}
	}
	return true;
}

GridApproximizer::Segment::Segment(int begin, int end)
	: QPair<int, int>(begin, end)
{
}

int GridApproximizer::Segment::length() const
{
	return second - first;
}

bool GridApproximizer::Segment::operator<(Segment const &rhs) const
{
	return first == rhs.first
			? second < rhs.second
			: first < rhs.first;
}

int GridApproximizer::minimum(QList<int> const &list)
{
	int result = INT_MAX;
	foreach (int element, list) {
		result = qMin(result, element);
	}
	return result;
}
