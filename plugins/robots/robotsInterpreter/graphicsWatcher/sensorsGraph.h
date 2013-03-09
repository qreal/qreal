#pragma once

#include <QtGui/QWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QToolButton>
#include <QtGui/QComboBox>
#include <QtCore/QTimer>

#include "../../../qrutils/expressionsParser/expressionsParser.h"
#include "sensorViewer.h"

namespace Ui {
class SensorsGraph;
}

//! @class SensorsGraph is widget to show sensors state
class SensorsGraph : public QWidget
{
	Q_OBJECT

public:
	explicit SensorsGraph(utils::ExpressionsParser const *parser, QWidget *parent = 0);
	~SensorsGraph();

public slots:
	void sensorsInput(int const port, qreal const value);
	void setMainSensor(int const newSlotIndex);
	void startJob();
	void stopJob();

protected:
	void initGui();
	void setupToolElements();
	void makeConnections();

private slots:
	void updateValues();

private:
	Ui::SensorsGraph *mUi;
	qReal::interpreters::robots::sensorsGraph::SensorViewer *mPlotFrame;
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
};
