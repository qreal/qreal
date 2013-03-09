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
	SensorViewer(QWidget *parent = 0);
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
	QGraphicsScene *scene;
	QTimer visualTimer;
	QList<QPointF> pointsQueue;
	KeyPoint *mainPoint;
	PointsQueueProcessor *pointsDataProcessor;
	QBrush mPenBrush;

	const int fpsDelay;
	const int autoScaleInterval;
	const int updateOutputInterval; //! current output value in bottom-right
	const int stepSize;
	const int zoomRate;
	int mScaleCoefficient;
	int mAutoScaleTimer;
	int mUpdateCurrValueTimer;
	qreal mOutputValue;
};

}
}
}
}

