#pragma once

#include <QtGui/QGraphicsView>
#include <QtCore/QTimer>
#include <QtGui/QPainter>

#include "keyPoint.h"
#include "pointsQueueProcessor.h"

namespace qReal
{
namespace interpreters
{
namespace robots
{
namespace sensorsGraph
{

class SensorViewer : public QGraphicsView
{
	Q_OBJECT

public:
	explicit SensorViewer(QWidget *parent = 0);
	~SensorViewer();

public slots:
	void setNextValue(qreal const newValue);
	void startJob();
	void stopJob();
	void clear();
	void zoomIn();
	void zoomOut();
	void onSensorChange(int const newSensorIndex);

protected:
	void drawNextFrame();
	void drawBackground(QPainter *painter, const QRectF &rect);

	void initGraphicsOutput();

private slots:
	void visualTimerEvent();

private:
	QGraphicsScene *mScene;
	QTimer mVisualTimer;
	KeyPoint *mainPoint;
	PointsQueueProcessor *mPointsDataProcessor;
	QBrush mPenBrush;

	static int const fpsDelay = 50;
	//! @variable autoScaleInterval time in milliseconds
	static int const autoScaleInterval = 4000;
	//! update interval of current value
	static int const updateOutputInterval = 800;
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
}
}

