#pragma once

#include <QtCore/QList>
#include <QtCore/QPointF>
#include <limits>

namespace utils {
namespace sensorsGraph {

/// @class PointsQueueProcessor provides all necessary transformations with points
/// Features: scaling by search of peaks on plot
/// Full recount of current plot with new min/max rate
/// Convertion absolute value to plot-Y-value and back
/// @remarks get out point list to draw plot with them
class PointsQueueProcessor
{
public:
	/// @param viewPortHeight takes amplitude for graphics without top and bottom bounds
	/// @param leftLimit takes sceneRect.left() for deleting out_to_date points
	PointsQueueProcessor(qreal const viewPortHeight, qreal const leftLimit);
	~PointsQueueProcessor();

	void addNewValue(qreal const newValue);
	void clearData();

	/// Shifts left points queue to animate plot
	/// use this func on each iteration
	/// @param step one shift in pixels
	void makeShiftLeft(const qreal step);

	/// function scales plot with current peaks
	/// This is part of autoscaling machine
	/// It might be a good idea to run this func once per 2-5 sec.
	void checkPeaks();

	/// @return QPointF intersection point of (func y = X) and mainplot
	QPointF pointOfVerticalIntersection(QPointF const &position) const;

	void setViewParams(qreal const viewPortHeight, qreal const leftLimit);

	/// get new position for KeyPoint
	QPointF latestPosition() const;
	qreal latestValue() const;

	/// get points data to draw a plot
	QList<QPointF> const &pointsBase() const;

	qreal minLimit() const;
	qreal maxLimit() const;

	qreal absoluteValueToPoint(qreal const value) const;

	qreal pointToAbsoluteValue(qreal const yValue) const;

protected:
	void recalcPointsQueue(qreal const oldMin, qreal const oldMax, qreal const oldViewHeight);
	/// Removes last values if they have no difference
	void filterLastValues();

	qreal pointToAbsoluteValue(qreal const yValue, qreal const minValue
			, qreal const maxValue, qreal const graphHeight) const;

	QList<QPointF> mPointsQueue;
	qreal mMinCurrent;
	qreal mMaxCurrent;
	QPointF mNextToDraw;
	qreal mGraphHeight;
	qreal mLeftLimit;
};

}
}
