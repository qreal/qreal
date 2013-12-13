#include "pointsQueueProcessor.h"

using namespace utils::sensorsGraph;

PointsQueueProcessor::PointsQueueProcessor(const qreal viewPortHeight, const qreal leftLimit)
	: mMinCurrent(0)
	, mMaxCurrent(1)
	, mNextToDraw(QPointF(0, -80))
{
	setViewParams(viewPortHeight, leftLimit);
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
		recalcPointsQueue(oldMin, oldMax, mGraphHeight);
	}

	mNextToDraw.setY(absoluteValueToPoint(newValue));
	filterLastValues();
	mPointsQueue.append(QPointF(0, absoluteValueToPoint(newValue)));
}

void PointsQueueProcessor::recalcPointsQueue(qreal const oldMin, qreal const oldMax, qreal const oldViewHeight)
{
	qreal currentValue = 0;
	for (int i = 0; i < mPointsQueue.size(); i++) {
		currentValue = pointToAbsoluteValue(mPointsQueue[i].y(), oldMin, oldMax, oldViewHeight);
		mPointsQueue[i].setY(absoluteValueToPoint(currentValue));
	}
}

void PointsQueueProcessor::filterLastValues()
{
	if (mPointsQueue.size() < 2) {
		return;
	}

	qreal const filterPixelDiff = 2;
	if (qAbs(mPointsQueue.at(mPointsQueue.size() - 2).y() - mPointsQueue.last().y()) < filterPixelDiff) {
		mPointsQueue.removeLast();
	}
}

void PointsQueueProcessor::makeShiftLeft(qreal const step)
{
	mPointsQueue.append(latestPosition());
	for (int i = 0; i < mPointsQueue.size(); i++) {
		mPointsQueue[i].setX(mPointsQueue[i].x() - step);
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

qreal PointsQueueProcessor::pointToAbsoluteValue(const qreal yValue) const
{
	return pointToAbsoluteValue(yValue, mMinCurrent, mMaxCurrent, mGraphHeight);
}

qreal PointsQueueProcessor::pointToAbsoluteValue(qreal const yValue, qreal const minValue
		, qreal const maxValue, qreal const graphHeight) const
{
	return (((maxValue - minValue) * (-yValue - 10)) / graphHeight) + minValue;
}

void PointsQueueProcessor::clearData()
{
	mMinCurrent = 0;
	mMaxCurrent = 1;
	mPointsQueue.clear();
}

QPointF PointsQueueProcessor::latestPosition() const
{
	return mNextToDraw;
}

qreal PointsQueueProcessor::latestValue() const
{
	return pointToAbsoluteValue(mNextToDraw.y(), mMinCurrent, mMaxCurrent, mGraphHeight);
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
	mMinCurrent = std::numeric_limits<qreal>::max();

	for (int i = 0; i < mPointsQueue.size(); i++) {
		if (pointToAbsoluteValue(mPointsQueue[i].y(), oldMin, oldMax, mGraphHeight) > mMaxCurrent) {
			mMaxCurrent = pointToAbsoluteValue(mPointsQueue[i].y(), oldMin, oldMax, mGraphHeight);
		}
		if (pointToAbsoluteValue(mPointsQueue[i].y(), oldMin, oldMax, mGraphHeight) < mMinCurrent) {
			mMinCurrent = pointToAbsoluteValue(mPointsQueue[i].y(), oldMin, oldMax, mGraphHeight);
		}
	}

	recalcPointsQueue(oldMin, oldMax, mGraphHeight);
}

QPointF PointsQueueProcessor::pointOfVerticalIntersection(QPointF const &position) const
{
	if (mPointsQueue.isEmpty())
		return QPointF(0, 0);
	qreal const accuracyCoefficient = 2; // accuracy = diameter * accCoefficient
	qreal const diameter = (mPointsQueue.last().x() - mPointsQueue.first().x()) / mPointsQueue.size();

	QPointF result;
	int leftBound = 0;
	int rightBound = mPointsQueue.size() - 1;
	while (leftBound < rightBound) {
		int middleIndex = (rightBound + leftBound) / 2;
		result = mPointsQueue.at(middleIndex);
		if (qAbs(result.x() - position.x()) < diameter * accuracyCoefficient) {
			break;
		}

		if (position.x() < result.x()) {
			rightBound = middleIndex - 1;
		}

		if (position.x() > result.x()) {
			leftBound = middleIndex + 1;
		}
	}

	return result;
}

void PointsQueueProcessor::setViewParams(qreal const viewPortHeight, qreal const leftLimit)
{
	qreal const oldViewHeight = mGraphHeight;
	mGraphHeight = viewPortHeight;
	recalcPointsQueue(mMinCurrent, mMaxCurrent, oldViewHeight);
	mLeftLimit = leftLimit;
}

qreal PointsQueueProcessor::minLimit() const
{
	return mMinCurrent;
}

qreal PointsQueueProcessor::maxLimit() const
{
	return mMaxCurrent;
}
