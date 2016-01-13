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

void PointsQueueProcessor::addNewValue(const qreal newValue)
{
	const qreal oldMax = mMaxCurrent;
	const qreal oldMin = mMinCurrent;
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

void PointsQueueProcessor::recalcPointsQueue(const qreal oldMin, const qreal oldMax, const qreal oldViewHeight)
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

	const qreal filterPixelDiff = 2;
	if (qAbs(mPointsQueue.at(mPointsQueue.size() - 2).y() - mPointsQueue.last().y()) < filterPixelDiff) {
		mPointsQueue.removeLast();
	}
}

void PointsQueueProcessor::makeShiftLeft(const qreal step)
{
	mPointsQueue.append(latestPosition());
	for (int i = 0; i < mPointsQueue.size(); i++) {
		mPointsQueue[i].setX(mPointsQueue[i].x() - step);
		if (mPointsQueue[i].x() < mLeftLimit) {
			mPointsQueue.removeAt(i--);
		}
	}
}

qreal PointsQueueProcessor::absoluteValueToPoint(const qreal value) const
{
	const int verticalBounds = 10;
	const int invertCoordSys = -1;
	if (qFuzzyCompare(mMaxCurrent, mMinCurrent)) {
		return mNextToDraw.y();
	}

	return ((value - mMinCurrent) / (mMaxCurrent - mMinCurrent) * mGraphHeight + verticalBounds) * invertCoordSys;
}

qreal PointsQueueProcessor::pointToAbsoluteValue(const qreal yValue) const
{
	return pointToAbsoluteValue(yValue, mMinCurrent, mMaxCurrent, mGraphHeight);
}

qreal PointsQueueProcessor::pointToAbsoluteValue(const qreal yValue, const qreal minValue
		, const qreal maxValue, const qreal graphHeight) const
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

const QList<QPointF> &PointsQueueProcessor::pointsBase() const
{
	return mPointsQueue;
}

void PointsQueueProcessor::checkPeaks()
{
	const qreal oldMin = mMinCurrent;
	const qreal oldMax = mMaxCurrent;

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

QPointF PointsQueueProcessor::pointOfVerticalIntersection(const QPointF &position) const
{
	if (mPointsQueue.isEmpty())
		return QPointF(0, 0);
	const qreal accuracyCoefficient = 2; // accuracy = diameter * accCoefficient
	const qreal diameter = (mPointsQueue.last().x() - mPointsQueue.first().x()) / mPointsQueue.size();

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

void PointsQueueProcessor::setViewParams(const qreal viewPortHeight, const qreal leftLimit)
{
	const qreal oldViewHeight = mGraphHeight;
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
