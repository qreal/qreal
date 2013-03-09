#include "pointsQueueProcessor.h"

using namespace qReal::interpreters::robots::sensorsGraph;

PointsQueueProcessor::PointsQueueProcessor(const qreal viewPortHeight, const qreal leftLimit)
	: mMinCurrent(0)
	, mMaxCurrent(1)
	, mNextToDraw(QPointF(0, -80))
	, mGraphHeight(viewPortHeight)
	, mLeftLimit(leftLimit)
{
}

PointsQueueProcessor::~PointsQueueProcessor()
{
	mPointsQueue.clear();
}

void PointsQueueProcessor::addNewValue(qreal const newValue)
{
	qreal const oldMax = mMaxCurrent;
	qreal const oldMin = mMinCurrent;
	if (newValue > mMaxCurrent) {
		mMaxCurrent = newValue;
	}
	if (newValue < mMinCurrent) {
		mMinCurrent = newValue;
	}

	if (oldMax != mMaxCurrent || oldMin != mMinCurrent) {
		recalcPointsQueue(oldMin, oldMax);
	}

	mNextToDraw.setY(absoluteValueToPoint(newValue));
	mPointsQueue.append(QPointF(0, absoluteValueToPoint(newValue)));
}

void PointsQueueProcessor::recalcPointsQueue(qreal const oldMin, qreal const oldMax)
{
	qreal currentValue = 0;
	for (int i = 0; i < mPointsQueue.size(); i++) {
		currentValue = pointToAbsoluteValue(mPointsQueue[i].y(), oldMin, oldMax);
		mPointsQueue[i].setY(absoluteValueToPoint(currentValue));
	}
}

void PointsQueueProcessor::makeShiftLeft(qreal const step)
{
	mPointsQueue.append(latestPosition());
	for (int i = 0; i < mPointsQueue.size(); i++) {
		mPointsQueue[i].setX(mPointsQueue[i].x() - step );
		if (mPointsQueue[i].x() < mLeftLimit) {
			mPointsQueue.removeAt(i--);
		}
	}
}

qreal PointsQueueProcessor::absoluteValueToPoint(qreal const value) const
{
	int const verticalBounds = 10;
	int const invertCoordSys = -1;
	return ((value - mMinCurrent) / (mMaxCurrent - mMinCurrent) * mGraphHeight + verticalBounds) * invertCoordSys;
}

qreal PointsQueueProcessor::pointToAbsoluteValue(const qreal yValue, const qreal minValue
		, const qreal maxValue) const
{
	return (((maxValue - minValue) * (-yValue - 10)) / mGraphHeight) + minValue;
}

void PointsQueueProcessor::clearData()
{
	mMinCurrent = 0;
	mMaxCurrent = 1;
	mPointsQueue.clear();
}

QPointF &PointsQueueProcessor::latestPosition()
{
	return mNextToDraw;
}

qreal PointsQueueProcessor::latestValue() const
{
	return pointToAbsoluteValue(mNextToDraw.y(), mMinCurrent, mMaxCurrent);
}

QList<QPointF> *PointsQueueProcessor::pointsBase()
{
	return &mPointsQueue;
}

void PointsQueueProcessor::checkPeaks()
{
	qreal const oldMin = mMinCurrent;
	qreal const oldMax = mMaxCurrent;

	mMaxCurrent = std::numeric_limits<qreal>::min();

	for (int i = 0; i < mPointsQueue.size(); i++) {
		if (pointToAbsoluteValue(mPointsQueue[i].y(), oldMin, oldMax) > mMaxCurrent) {
			mMaxCurrent = pointToAbsoluteValue(mPointsQueue[i].y(), oldMin, oldMax);
		}
		if (pointToAbsoluteValue(mPointsQueue[i].y(), oldMin, oldMax) < mMinCurrent) {
			mMinCurrent = pointToAbsoluteValue(mPointsQueue[i].y(), oldMin, oldMax);
		}
	}

	recalcPointsQueue(oldMin, oldMax);
}

qreal PointsQueueProcessor::minLimit() const
{
	return mMinCurrent;
}

qreal PointsQueueProcessor::maxLimit() const
{
	return mMaxCurrent;
}
