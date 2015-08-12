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

#include "keyBuilder.h"

#include <qrutils/mathUtils/math.h>

static const int minMovement = 20;
static const qreal maxRelation = 8;
static const int minPoint = -1000;

using namespace qReal::gestures;

Key KeyBuilder::getKey(const PathVector &mousePath, int heightSize, int widthSize)
{
	Key key;
	if (mousePath.isEmpty()) {
		return key;
	}

	qreal lower = mousePath.at(0).at(0).y();
	qreal upper = mousePath.at(0).at(0).y();
	qreal right = mousePath.at(0).at(0).x();
	qreal left = mousePath.at(0).at(0).x();
	for (const PointVector &path : mousePath) {
		for (const QPointF &pnt : path) {
			lower = qMax(lower, pnt.y());
			upper = qMin(upper, pnt.y());
			right = qMax(right, pnt.x());
			left = qMin(left, pnt.x());
		}
	}

	if (right - left < minMovement && lower - upper < minMovement) {
		return key;
	}

	for (const PointVector &path : mousePath) {
		SquarePos previous(minPoint, minPoint);
		SquarePos last;

		for (const QPointF &point : path) {
			if ((lower - upper) * maxRelation  < right - left) {
				last.first = (point.x() - left) * widthSize / (right - left);
				last.second = 0;
			} else if ((right - left) * maxRelation < lower - upper) {
				last.first = 0;
				last.second = (point.y() - upper) * heightSize / (lower - upper);
			} else {
				last.first = static_cast<int>((point.x() - left) * widthSize / (right - left));
				last.second = static_cast<int>((point.y() - upper) * heightSize / (lower - upper));
			}

			if (last.first > widthSize) {
				last.first = widthSize;
			}

			if (last.second > heightSize) {
				last.second = heightSize;
			}

			if (previous.first != minPoint || previous.second != minPoint) {
				rasterizeSegment(previous, last, &key);
			}

			previous = last;
		}
	}

	return key;
}

void KeyBuilder::rasterizeSegment(const SquarePos &pos1, const SquarePos &pos2, Key *segment)
{
	if (!segment->isEmpty() && pos1 == segment->at(0)) {
		segment->pop_back();
	}

	if (pos1 == pos2) {
		segment->push_back(pos1);
		return;
	}

	int x = pos1.first;
	int y = pos1.second;
	int deltaX = qAbs(pos2.first - x);
	int deltaY = qAbs(pos2.second - y);
	const int signX = mathUtils::Math::sign(pos2.first - x);
	const int signY = mathUtils::Math::sign(pos2.second - y);
	bool isChanged = false;

	if (deltaY > deltaX) {
		int c = deltaX;
		deltaX = deltaY;
		deltaY = c;
		isChanged = true;
	}

	int e = 2 * deltaY - deltaX;

	for (int i = 0; i < deltaX; ++i) {
		segment->push_back(SquarePos(x, y));
		while (e >= 0) {
			if (isChanged) {
				x += signX;
			} else {
				y += signY;
			}

			e -= 2 * deltaX;
		}

		if (isChanged) {
			y += signY;
		} else {
			x += signX;
		}

		e += 2 * deltaY;
	}
}
