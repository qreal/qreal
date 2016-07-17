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

#include "gesturePainter.h"

#include <QtGui/QPainter>

#include "mouseMovementManager.h"

using namespace qReal::gestures;

const int thickness = 2;
/// Difference between painter's and pixmap's coordinates.
const qreal frame = 10;

GesturePainter::GesturePainter(const QString &gesture
		, const QColor &backgroungColor
		, const QColor &gestureColor
		, qreal size)
	: mGesture(MouseMovementManager::stringToPath(gesture))
	, mBackgroundColor(backgroungColor)
	, mGestureColor(gestureColor)
	, mSize(size)
{
}

QIconEngine *GesturePainter::clone() const
{
	return nullptr;
}

void GesturePainter::paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state)
{
	Q_UNUSED(rect)
	Q_UNUSED(mode)
	Q_UNUSED(state)

	qreal maxSizeX = 0;
	qreal maxSizeY = 0;
	for (const PointVector &path : mGesture) {
		if (path.isEmpty()) {
			return;
		}

		for (const QPointF &currentPoint : path) {
			maxSizeX = qMax(maxSizeX, currentPoint.x());
			maxSizeY = qMax(maxSizeY, currentPoint.y());
		}
	}

	if (maxSizeX == 0 || maxSizeY == 0) {
		// In case of empty gesture.
		maxSizeX = mSize;
		maxSizeY = mSize;
	}

	const qreal sizeFactor = mSize ? mSize / (qMax(maxSizeX, maxSizeY) + frame) : 1;
	const QPoint indent = QPoint((mSize - maxSizeX * sizeFactor) / 2, (mSize - maxSizeY * sizeFactor) / 2);

	painter->save();
	painter->setBrush(mBackgroundColor);
	painter->setPen(Qt::white);
	painter->drawRect(0, 0, mSize, mSize);

	QPen pen(mGestureColor);
	pen.setWidth(thickness);
	painter->setPen(pen);
	for (const PointVector &path : mGesture) {
		QPointF previousPoint = path[0];
		for (const QPointF &currentPoint : path) {
			painter->drawLine(indent + sizeFactor * previousPoint, indent + sizeFactor * currentPoint);
			previousPoint = currentPoint;
		}
	}

	painter->restore();
}
