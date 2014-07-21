#include "gesturePainter.h"

#include <view/gestures/mouseMovementManager.h>
#include <mainwindow/gesturesPainterInterface.h>
#include <math.h>

using namespace qReal::gestures;

GesturePainter::GesturePainter(QString const &gesture, QColor backgroungColor, QColor gestColor, qreal height) :
	mGesture(gesture), mBackgroundColor(backgroungColor), mGestColor(gestColor), mHeight(height)
{
}

GesturePainter::GesturePainter(QString const &gesture) : mGesture(gesture)
{
	mBackgroundColor = Qt::transparent;
	mGestColor = Qt::blue;
	mHeight = 0;
}

QIconEngine* GesturePainter::clone() const
{
	return (nullptr);
}

void GesturePainter::paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state)
{
	Q_UNUSED(rect);
	Q_UNUSED(mode);
	PathVector paths = MouseMovementManager::stringToPath(this->mGesture);
	qreal maxSizeX = 0;
	qreal maxSizeY = 0;
	foreach (PointVector const &path, paths) {
		if (path.isEmpty()) {
			return;
		}
		foreach (QPointF const &currentPoint, path) {
			maxSizeX = qMax(maxSizeX, currentPoint.x());
			maxSizeY = qMax(maxSizeY, currentPoint.y());
		}
	}
	if (maxSizeX == 0 || maxSizeY == 0) {
		//in case of empty gesture.
		maxSizeX = mHeight;
		maxSizeY = mHeight;
	}
	qreal sizeFactor;
	if (mHeight != 0) {
		sizeFactor = mHeight / (qMax(maxSizeX, maxSizeY) + mFrame);
	} else {
		sizeFactor = 1;
	}
	QPoint indent = QPoint((mHeight - maxSizeX * sizeFactor) / 2, (mHeight - maxSizeY * sizeFactor) / 2);
	painter->setBrush(mBackgroundColor);
	painter->setPen(Qt::white);
	painter->drawRect(0, 0, mHeight, mHeight);
	QPen pen(mGestColor);
	pen.setWidth(mGestWidth);
	painter->setPen(pen);
	foreach (PointVector const &path, paths) {
		QPointF previousPoint(minBoarder, minBoarder);
		if (path.isEmpty()) {
			return;
		}
		foreach (QPointF const &currentPoint, path) {
			if (previousPoint.x() != minBoarder && previousPoint.y() != minBoarder) {
				painter->drawLine(indent + sizeFactor * previousPoint, indent + sizeFactor * currentPoint);
			} else {
				painter->drawLine(indent + sizeFactor * currentPoint, indent + sizeFactor * currentPoint);
			}
			previousPoint = currentPoint;
		}
	}
}
