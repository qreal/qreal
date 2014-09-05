#include "keyBuilder.h"

#include <qrutils/mathUtils/math.h>

static const int minMovement = 20;
static const double maxRelation = 8;
static const int minPoint = -1000;

using namespace qReal::gestures;

Key KeyBuilder::getKey(PathVector const &mousePath, int heightSize, int widthSize)
{
	Key key;
	if (mousePath.isEmpty()) {
		return key;
	}

	qreal lower = mousePath.at(0).at(0).y();
	qreal upper = mousePath.at(0).at(0).y();
	qreal right = mousePath.at(0).at(0).x();
	qreal left = mousePath.at(0).at(0).x();
	for (PointVector const &path : mousePath) {
		for (QPointF const &pnt : path) {
			lower = qMax(lower, pnt.y());
			upper = qMin(upper, pnt.y());
			right = qMax(right, pnt.x());
			left = qMin(left, pnt.x());
		}
	}

	if (right - left < minMovement && lower - upper < minMovement) {
		return key;
	}

	for (PointVector const &path : mousePath) {
		SquarePos previous(minPoint, minPoint);
		SquarePos last;

		for (QPointF const &point : path) {
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

void KeyBuilder::rasterizeSegment(SquarePos const &pos1, SquarePos const &pos2, Key *segment)
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
	int deltaX = abs(pos2.first - x);
	int deltaY = abs(pos2.second - y);
	int const signX = mathUtils::Math::sign(pos2.first - x);
	int const signY = mathUtils::Math::sign(pos2.second - y);
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
