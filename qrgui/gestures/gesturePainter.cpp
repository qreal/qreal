#include "gesturePainter.h"

#include "mouseMovementManager.h"

using namespace qReal::gestures;

int const minBorder = -1000;
int const thickness = 2;
/// Difference between painter's and pixmap's coordinates.
qreal const frame = 10;

GesturePainter::GesturePainter(QString const &gesture
		, QColor const &backgroungColor
		, QColor const &gestureColor
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

void GesturePainter::paint(QPainter *painter, QRect const &rect, QIcon::Mode mode, QIcon::State state)
{
	Q_UNUSED(rect)
	Q_UNUSED(mode)
	Q_UNUSED(state)

	qreal maxSizeX = 0;
	qreal maxSizeY = 0;
	for (PointVector const &path : mGesture) {
		if (path.isEmpty()) {
			return;
		}

		for (QPointF const &currentPoint : path) {
			maxSizeX = qMax(maxSizeX, currentPoint.x());
			maxSizeY = qMax(maxSizeY, currentPoint.y());
		}
	}

	if (maxSizeX == 0 || maxSizeY == 0) {
		// In case of empty gesture.
		maxSizeX = mSize;
		maxSizeY = mSize;
	}

	qreal const sizeFactor = mSize ? mSize / (qMax(maxSizeX, maxSizeY) + frame) : 1;
	QPoint const indent = QPoint((mSize - maxSizeX * sizeFactor) / 2, (mSize - maxSizeY * sizeFactor) / 2);

	painter->save();
	painter->setBrush(mBackgroundColor);
	painter->setPen(Qt::white);
	painter->drawRect(0, 0, mSize, mSize);

	QPen pen(mGestureColor);
	pen.setWidth(thickness);
	painter->setPen(pen);
	for (PointVector const &path : mGesture) {
		QPointF previousPoint = path[0];
		for (QPointF const &currentPoint : path) {
			painter->drawLine(indent + sizeFactor * previousPoint, indent + sizeFactor * currentPoint);
			previousPoint = currentPoint;
		}
	}

	painter->restore();
}
