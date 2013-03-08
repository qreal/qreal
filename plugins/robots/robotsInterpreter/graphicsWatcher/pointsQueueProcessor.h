#pragma once

#include <qglobal.h>
#include <QList>
#include <QPointF>
#include <limits>

//! @class PointsQueueProcessor provides all necessary transformations with points
/*! Features: scaling by search of peaks on plot
	Full recount of current plot with new min/max rate
	Convertion absolute value to plot-Y-value and back
	@remarks get out point list to draw plot with them
*/
class PointsQueueProcessor
{
public:
	/*!
	  @param viewPortHeight takes amplitude for graphics without top and bottom bounds
	  @param leftLimit takes sceneRect.left() for deleting out_to_date points
	*/
	PointsQueueProcessor(qreal const viewPortHeight, qreal const mLeftLimit);
	~PointsQueueProcessor();

	void addNewValue(qreal const newValue);
	void clearData();

	//! Shifts left points queue to animate plot
	/*! use this func on each iterate
	  @param step one shift in pixels
	*/
	void makeShiftLeft(const qreal step);

	//! function scaled plot with current pics
	/*! This is part of autoscaling machine
		It might be a good idea to run this func ones per 2-5 sec.
	*/
	void checkPeaks();

	//! get new position for KeyPoint
	QPointF &latestPosition();
	qreal latestValue();

	//! get points data to draw a plot
	QList<QPointF> *pointsBase();

	qreal minLimit();
	qreal maxLimit();

	qreal absoluteValueToPoint(qreal const value) const;
	qreal pointToAbsoluteValue(qreal const yValue, qreal const minValue, qreal const maxValue) const;


protected:
	QList<QPointF> mPointsQueue;
	qreal mMinCurrent;
	qreal mMaxCurrent;
	QPointF mNextToDraw;
	qreal mGraphHeight;
	qreal mLeftLimit;

	void recalcPointsQueue(qreal const oldMin, qreal const oldMax);


};

