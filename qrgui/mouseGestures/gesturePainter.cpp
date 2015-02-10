#include "gesturePainter.h"

#include "mouseMovementManager.h"

using namespace qReal::gestures;

const int minBorder = -1000;
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
