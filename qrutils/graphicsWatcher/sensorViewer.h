#pragma once

#include <QtWidgets/QGraphicsView>
#include <QtCore/QTimer>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QToolTip>

#include "keyPoint.h"
#include "pointsQueueProcessor.h"

namespace utils {
namespace sensorsGraph {

class SensorViewer : public QGraphicsView
{
	Q_OBJECT

public:
	explicit SensorViewer(QWidget *parent = 0);
	~SensorViewer();

	//! all intervals in ms
	void configureUserOptions(int const &fpsDelay, int const &autoScaleDelay, int const &textInfoUpdateDelay);

	void resizeEvent(QResizeEvent *event = 0);

public slots:
	void setNextValue(qreal const newValue);
	void startJob();
	void stopJob();
	void clear();
	void zoomIn();
	void zoomOut();
	void onSensorChange();

protected:
	void drawNextFrame();
	void drawBackground(QPainter *painter, const QRectF &rect);
	//! renders hint with value under cursor
	void mouseMoveEvent(QMouseEvent *event);
	void leaveEvent(QEvent *);
	void mouseDoubleClickEvent(QMouseEvent *event);

	void initGraphicsOutput();

private slots:
	void visualTimerEvent();

private:
	QGraphicsScene *mScene;
	QTimer mVisualTimer;
	KeyPoint *mMainPoint;
	KeyPoint *mMarker;
	PointsQueueProcessor *mPointsDataProcessor;
	QBrush mPenBrush;

	//! output update delay in ms
	int mFpsInterval;
	//! @variable autoScaleInterval time in milliseconds
	int mAutoScaleInterval;
	//! update interval of current value
	int mUpdateTextInfoInterval;
	//! shift in pixels each frame
	static int const stepSize = 2;
	//! zoom coefficent
	static int const zoomRate = 2;

	int mScaleCoefficient;
	int mAutoScaleTimer;
	int mUpdateCurrValueTimer;
	qreal mOutputValue;
};

}
}

