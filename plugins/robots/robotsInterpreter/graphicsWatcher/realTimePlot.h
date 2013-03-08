#pragma once

#include <QtGui/QWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QToolButton>
#include <QtGui/QComboBox>
#include <QTimer>

#include "../../../qrutils/expressionsParser/expressionsParser.h"
#include "sensorGraph.h"

namespace Ui {
class RealTimePlot;
}

class RealTimePlot : public QWidget
{
	Q_OBJECT

public:
	explicit RealTimePlot(utils::ExpressionsParser const *parser, QWidget *parent = 0);
	~RealTimePlot();

public slots:
	void sensorOne(qreal const value);
	void sensorTwo(qreal const value);
	void sensorThree(qreal const value);
	void sensorFour(qreal const value);
	void setMainSensor(int const newSlotIndex);
	void startJob();
	void stopJob();


protected:
	void initGui();
	void setupToolElements();
	void makeConnections();

private:
	Ui::RealTimePlot *mUi;
	SensorGraph *mPlotFrame;
	QVBoxLayout mToolLayout;
	QToolButton mStopButton;
	QToolButton mStartButton;
	QToolButton mZoomInButton;
	QToolButton mZoomOutButton;
	QToolButton mResetButton;
	QComboBox mSlotComboBox;
	QTimer mUpdateTimer;
	utils::ExpressionsParser const *mParser;
	int const mUpdateRate;
	int mCurrentSlot;

private slots:
	void updateValues();

};

