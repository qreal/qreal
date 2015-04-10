/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
	PointsQueueProcessor(const qreal viewPortHeight, const qreal leftLimit);
	~PointsQueueProcessor();

	void addNewValue(const qreal newValue);
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
	QPointF pointOfVerticalIntersection(const QPointF &position) const;

	void setViewParams(const qreal viewPortHeight, const qreal leftLimit);

	/// get new position for KeyPoint
	QPointF latestPosition() const;
	qreal latestValue() const;

	/// get points data to draw a plot
	const QList<QPointF> &pointsBase() const;

	qreal minLimit() const;
	qreal maxLimit() const;

	qreal absoluteValueToPoint(const qreal value) const;

	qreal pointToAbsoluteValue(const qreal yValue) const;

protected:
	void recalcPointsQueue(const qreal oldMin, const qreal oldMax, const qreal oldViewHeight);
	/// Removes last values if they have no difference
	void filterLastValues();

	qreal pointToAbsoluteValue(const qreal yValue, const qreal minValue
			, const qreal maxValue, const qreal graphHeight) const;

	QList<QPointF> mPointsQueue;
	qreal mMinCurrent;
	qreal mMaxCurrent;
	QPointF mNextToDraw;
	qreal mGraphHeight;
	qreal mLeftLimit;
};

}
}
