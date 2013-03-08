#pragma once

#include <QGraphicsView>
#include <QTimer>
#include <QPainter>

#include "keyPoint.h"
#include "pointsQueueProcessor.h"

class SensorGraph : public QGraphicsView
{
	Q_OBJECT

public:
	SensorGraph(QWidget *parent = 0);
	~SensorGraph();

signals:

public slots:
	void setNextValue(qreal newValue);
	void startJob();
	void stopJob();
	void clear();
	void zoomIn();
	void zoomOut();
	void onSensorChange(int const newSensorIndex);

protected:
	void drawNextFrame();
	void drawBackground(QPainter *painter, const QRectF &rect);
	void mouseReleaseEvent(QMouseEvent *event);

	void initGraphicsOutput();

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
	bool mIsZoomed;
	int mScaleCoefficient;
	int mAutoScaleTimer;
	int mUpdateCurrValueTimer;
	qreal mOutputValue;


	/// debug
	qreal debugValue;

private slots:
	void visualTimerEvent();

};

